/* t2os_test: Turok 2's thread graph on t2os (port/runtime), driven like the LUS main loop will
 * drive it: per host frame, post the completions of last frame's RSP tasks, one VI retrace, then
 * run the game threads until they are all blocked.
 *
 * The threads, priorities, queues and message flow are the engine's (docs/port/RUNTIME.md §2):
 * main (CEngineApp__Main, 13), scheduler (__scMain, 18), loader (CLoader__Main, 17), cache
 * (CCache__Main, 15), decompressor (CDecompressor__Main, 11, raised to 14 for a sync request),
 * audio (__amMain, 16). The bodies are stand-ins, not the game's code.
 *
 * Checks: libultra preemption (a send to a higher-priority thread runs it before the send
 * returns), priority boost, frame and audio counts after N ticks, and a hash of the thread switch
 * order, which must be the same on every run (determinism). Also measures switch cost.
 */
#include "../runtime/t2os.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TICKS 600

static int fails;
#define CHECK(c, ...)                         \
    do {                                      \
        if (!(c)) {                           \
            fails++;                          \
            fprintf(stderr, "FAIL: " __VA_ARGS__); \
            fprintf(stderr, "\n");            \
        }                                     \
    } while (0)

/* trace hash (FNV-1a over the switch sequence) */
static u64 sHash = 1469598103934665603ull;
static void trace(int ev, OSId from, OSId to) {
    u32 w = (u32)(ev & 0xFF) | ((u32)(from & 0xFF) << 8) | ((u32)(to & 0xFF) << 16);
    for (int i = 0; i < 4; i++) {
        sHash ^= (w >> (i * 8)) & 0xFF;
        sHash *= 1099511628211ull;
    }
}

/* ---- fake ROM and loader (CLoader) ---- */
static unsigned char sRom[1 << 16];
typedef struct {
    u32 address;
    void *dest;
    s32 length;
    OSMesgQueue *replyQueue;
    OSMesg replyMessage;
    int done;
} LoaderEntry;
static OSThread sLoaderThread;
static OSMesgQueue sLoaderQueue;
static OSMesg sLoaderMsgs[16];
static int sLoads;

static void loader_main(void *arg) {
    (void)arg;
    for (;;) {
        LoaderEntry *e;
        osRecvMesg(&sLoaderQueue, (OSMesg *)&e, OS_MESG_BLOCK);
        memcpy(e->dest, sRom + e->address, (size_t)e->length); /* T2Asset_ReadInto in the port */
        e->done = 1;
        sLoads++;
        if (e->replyQueue)
            osSendMesg(e->replyQueue, e->replyMessage, OS_MESG_BLOCK);
    }
}

/* CLoader__LoadSync: jam the request, wait for the reply */
static void load_sync(u32 address, s32 length, void *dest) {
    OSMesgQueue q;
    OSMesg m, r;
    LoaderEntry e = { address, dest, length, &q, (OSMesg)1, 0 };
    osCreateMesgQueue(&q, &m, 1);
    osJamMesg(&sLoaderQueue, &e, OS_MESG_BLOCK);
    /* libultra semantics: the loader (17) outranks the caller (13), so it has already run */
    CHECK(e.done == 1, "loader did not preempt the caller on osJamMesg");
    osRecvMesg(&q, &r, OS_MESG_BLOCK);
}

/* ---- decompressor (CDecompressor) and cache (CCache) ---- */
typedef struct {
    int value;
    int result;
    OSMesgQueue *replyQueue;
} CacheEntry;
static OSThread sDecompThread, sCacheThread;
static OSMesgQueue sDecompQueue, sCacheQueue;
static OSMesg sDecompMsgs[8], sCacheMsgs[8];
static int sDecompressions, sDecompPriSeen;

static void decomp_main(void *arg) {
    (void)arg;
    for (;;) {
        CacheEntry *e;
        osRecvMesg(&sDecompQueue, (OSMesg *)&e, OS_MESG_BLOCK);
        sDecompPriSeen = osGetThreadPri(NULL);
        e->result = e->value * 3 + 1; /* the entry's decompress callback (RNC, model unpack...) */
        sDecompressions++;
        osSendMesg(e->replyQueue, e, OS_MESG_BLOCK);
    }
}

/* CDecompressor__DecompressSync: raise the decompressor to 14 and jam */
static int decompress_sync(int v) {
    OSMesgQueue q;
    OSMesg m, r;
    CacheEntry e = { v, 0, &q };
    osCreateMesgQueue(&q, &m, 1);
    osSetThreadPri(&sDecompThread, 14);
    osJamMesg(&sDecompQueue, &e, OS_MESG_BLOCK);
    osRecvMesg(&q, &r, OS_MESG_BLOCK);
    osSetThreadPri(&sDecompThread, 11);
    return e.result;
}

static int sCacheCompleted;
static void cache_main(void *arg) {
    (void)arg;
    for (;;) {
        CacheEntry *e;
        osRecvMesg(&sCacheQueue, (OSMesg *)&e, OS_MESG_BLOCK);
        if (e->replyQueue == &sCacheQueue) { /* back from the decompressor: complete the request */
            sCacheCompleted++;
            continue;
        }
        e->replyQueue = &sCacheQueue; /* CDecompressor__DecompressAsync(&gDecompressor, msg, cache queue) */
        osSendMesg(&sDecompQueue, e, OS_MESG_BLOCK);
    }
}

/* ---- scheduler (osCreateScheduler / __scMain) ---- */
enum { MSG_RETRACE = 0x29A, MSG_RSP_DONE = 0x29B, MSG_RDP_DONE = 0x29C, MSG_PRENMI = 0x29D, MSG_CMD = 6 };
typedef struct {
    s16 type;
} ScMsg;
typedef struct ScTask {
    int isGfx;
    OSMesgQueue *msgQ;
    OSMesg msg;
} ScTask;
static OSThread sSchedThread;
static OSMesgQueue sSchedQueue, sSchedCmdQueue;
static OSMesg sSchedMsgs[8], sSchedCmdMsgs[8];
static ScMsg sRetraceMsg = { 1 };
static OSMesgQueue *sClients[2];
static int sNumClients;
static ScTask *sCurGfx, *sCurAudio;
static int sPendingGfxDone; /* the shim's queue of gfx completions, delivered at the next tick */
static int sGfxRun, sAudioRun;

/* the shim's osSpTaskStartGo: the display list goes to Fast3D (or the audio ABI to the HLE) at once */
static void sp_task_start_go(ScTask *t) {
    if (t->isGfx) {
        sGfxRun++; /* Fast3D: DrawAndRunGraphicsCommands(t->data_ptr) */
        sPendingGfxDone++;
    } else {
        sAudioRun++; /* audio HLE over the Acmd list */
        t2os_post_event(OS_EVENT_SP); /* audio completes at once */
    }
}

static void sched_main(void *arg) {
    (void)arg;
    for (;;) {
        OSMesg m;
        osRecvMesg(&sSchedQueue, &m, OS_MESG_BLOCK);
        switch ((int)(intptr_t)m) {
        case MSG_RETRACE:
            for (int i = 0; i < sNumClients; i++)
                osSendMesg(sClients[i], &sRetraceMsg, OS_MESG_NOBLOCK);
            break;
        case MSG_CMD: {
            ScTask *t;
            while (osRecvMesg(&sSchedCmdQueue, (OSMesg *)&t, OS_MESG_NOBLOCK) != -1) {
                if (t->isGfx)
                    sCurGfx = t;
                else
                    sCurAudio = t;
                sp_task_start_go(t);
            }
            break;
        }
        case MSG_RSP_DONE:
            if (sCurAudio) { /* audio task done: reply to the audio manager */
                ScTask *t = sCurAudio;
                sCurAudio = NULL;
                osSendMesg(t->msgQ, t->msg, OS_MESG_BLOCK);
            }
            break;
        case MSG_RDP_DONE:
            if (sCurGfx) { /* __scHandleRDP: frame done, tell the engine */
                ScTask *t = sCurGfx;
                sCurGfx = NULL;
                osSendMesg(t->msgQ, t->msg, OS_MESG_BLOCK);
            }
            break;
        }
    }
}

static void sched_submit(ScTask *t) { /* Sched_SubmitTask */
    osSendMesg(&sSchedCmdQueue, t, OS_MESG_BLOCK);
    osSendMesg(&sSchedQueue, (OSMesg)(intptr_t)MSG_CMD, OS_MESG_BLOCK);
}

/* ---- audio manager (__amMain / __amHandleFrameMsg) ---- */
static OSThread sAudioThread;
static OSMesgQueue sAudioFrameQueue, sAudioReplyQueue;
static OSMesg sAudioFrameMsgs[10], sAudioReplyMsgs[10];
static int sAudioFrames;
static void audio_main(void *arg) {
    (void)arg;
    ScTask t = { 0, &sAudioReplyQueue, (OSMesg)1 };
    for (;;) {
        OSMesg m, r;
        osRecvMesg(&sAudioFrameQueue, &m, OS_MESG_BLOCK);
        /* alAudioFrame builds the Acmd list on the CPU, the RSP runs it */
        sched_submit(&t);
        osRecvMesg(&sAudioReplyQueue, &r, OS_MESG_BLOCK);
        sAudioFrames++; /* osAiSetNextBuffer -> AudioPlayerPlayFrame */
    }
}

/* ---- engine main thread (mainproc / CEngineApp__Main) ---- */
static OSThread sMainThread;
static OSMesgQueue sGfxQueue;
static OSMesg sGfxMsgs[64];
static ScMsg sDoneMsg = { 2 };
static int sFrames, sDisplayListCount;

static void main_proc(void *arg) {
    (void)arg;
    unsigned char buf[256];
    /* InitializeSystemResources */
    osCreateThread(&sDecompThread, 3, decomp_main, NULL, NULL, 11);
    osStartThread(&sDecompThread);
    osCreateThread(&sLoaderThread, 4, loader_main, NULL, NULL, 17);
    osStartThread(&sLoaderThread);
    osCreateThread(&sCacheThread, 5, cache_main, NULL, NULL, 15);
    osStartThread(&sCacheThread);
    /* CEngineApp__Construct: osCreateScheduler + osScAddClient */
    osCreateMesgQueue(&sGfxQueue, sGfxMsgs, 64);
    osSetEventMesg(OS_EVENT_SP, &sSchedQueue, (OSMesg)(intptr_t)MSG_RSP_DONE);
    osSetEventMesg(OS_EVENT_DP, &sSchedQueue, (OSMesg)(intptr_t)MSG_RDP_DONE);
    osSetEventMesg(OS_EVENT_PRENMI, &sSchedQueue, (OSMesg)(intptr_t)MSG_PRENMI);
    osViSetEvent(&sSchedQueue, (OSMesg)(intptr_t)MSG_RETRACE, 1);
    osCreateThread(&sSchedThread, 7, sched_main, NULL, NULL, 18);
    osStartThread(&sSchedThread);
    sClients[sNumClients++] = &sGfxQueue;
    /* CAudioWorld__Initialize -> amCreateAudioMgr (adds itself as a scheduler client) */
    osCreateThread(&sAudioThread, 8, audio_main, NULL, NULL, 16);
    osStartThread(&sAudioThread);
    sClients[sNumClients++] = &sAudioFrameQueue;

    /* boot-time loads and a sync decompress */
    for (int i = 0; i < 8; i++) {
        load_sync((u32)(i * 256), 256, buf);
        CHECK(buf[0] == sRom[i * 256] && buf[255] == sRom[i * 256 + 255], "load %d bytes wrong", i);
    }
    CHECK(decompress_sync(5) == 16, "sync decompress result");
    CHECK(sDecompPriSeen == 14, "decompressor ran at %d, not the boosted 14", sDecompPriSeen);

    sDisplayListCount = 2;
    ScTask gfx[3] = { { 1, &sGfxQueue, &sDoneMsg }, { 1, &sGfxQueue, &sDoneMsg }, { 1, &sGfxQueue, &sDoneMsg } };
    static CacheEntry req[TICKS];
    for (;;) {
        ScMsg *m;
        osRecvMesg(&sGfxQueue, (OSMesg *)&m, OS_MESG_BLOCK);
        if (m->type == 1) { /* CEngineApp__Retrace */
            if (sDisplayListCount != 0) {
                sDisplayListCount--;
                /* CEngineApp__Update -> Draw: an async cache request, then the frame */
                CacheEntry *e = &req[sFrames % TICKS];
                e->value = sFrames;
                e->replyQueue = NULL;
                osSendMesg(&sCacheQueue, e, OS_MESG_BLOCK);
                sched_submit(&gfx[sFrames % 3]); /* CEngineApp__SubmitGfxTask */
                sFrames++;
            }
        } else if (m->type == 2) { /* a frame finished */
            sDisplayListCount++;
        }
    }
}

int main(int argc, char **argv) {
    int ticks = argc > 1 ? atoi(argv[1]) : TICKS;
    struct timespec t0, t1;
    for (unsigned i = 0; i < sizeof(sRom); i++)
        sRom[i] = (unsigned char)(i * 7 + (i >> 8));
    t2os_init();
    t2os_set_trace(trace);
    osCreateMesgQueue(&sSchedQueue, sSchedMsgs, 8);
    osCreateMesgQueue(&sSchedCmdQueue, sSchedCmdMsgs, 8);
    osCreateMesgQueue(&sLoaderQueue, sLoaderMsgs, 16);
    osCreateMesgQueue(&sDecompQueue, sDecompMsgs, 8);
    osCreateMesgQueue(&sCacheQueue, sCacheMsgs, 8);
    osCreateMesgQueue(&sAudioFrameQueue, sAudioFrameMsgs, 10);
    osCreateMesgQueue(&sAudioReplyQueue, sAudioReplyMsgs, 10);

    /* the port's boot: no idle thread, the main thread is started directly */
    osCreateThread(&sMainThread, 1, main_proc, NULL, NULL, 13);
    osStartThread(&sMainThread);
    int boot = t2os_run_until_idle();

    clock_gettime(CLOCK_MONOTONIC, &t0);
    int switches = 0;
    for (int tick = 0; tick < ticks; tick++) {
        /* platform loop, once per host frame (LUS): last frame's gfx completions, then VI */
        while (sPendingGfxDone > 0) {
            sPendingGfxDone--;
            t2os_post_event(OS_EVENT_SP);
            t2os_post_event(OS_EVENT_DP);
        }
        t2os_vi_retrace();
        switches += t2os_run_until_idle();
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double secs = (double)(t1.tv_sec - t0.tv_sec) + (double)(t1.tv_nsec - t0.tv_nsec) * 1e-9;

    printf("boot: %d switches, %d loads, %d decompressions\n", boot, sLoads, sDecompressions);
    printf("%d ticks: %d frames submitted, %d gfx tasks run, %d audio frames, %d cache requests completed\n", ticks,
           sFrames, sGfxRun, sAudioFrames, sCacheCompleted);
    printf("%d thread switches, %.1f us per tick, %.2f us per switch\n", switches, secs * 1e6 / ticks,
           switches ? secs * 1e6 / switches : 0.0);
    printf("trace hash %016llx\n", (unsigned long long)sHash);
    CHECK(sFrames == ticks, "frames %d != ticks %d (one frame per retrace while the RSP keeps up)", sFrames, ticks);
    CHECK(sAudioFrames == ticks, "audio frames %d != ticks %d", sAudioFrames, ticks);
    CHECK(sCacheCompleted == sFrames, "cache requests completed %d != %d", sCacheCompleted, sFrames);
    if (fails) {
        t2os_dump();
        printf("FAILED (%d)\n", fails);
        return 1;
    }
    printf("OK\n");
    return 0;
}
