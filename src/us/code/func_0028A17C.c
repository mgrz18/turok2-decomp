/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028EAAC.c, func_0028EAAC, with Turok 2's layouts. */

#include "common.h"
#include "globals/render.h"

typedef struct Client {
    /* 0x0 */ struct Client *next;
    /* 0x4 */ void *queue;
} Client;

typedef struct ScTask {
    /* 0x00 */ struct ScTask *next;
    /* 0x04 */ s32 state;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 type;
} ScTask;

typedef struct {
    /* 0x000 */ u8 pad0[0x78];
    /* 0x078 */ u8 cmdQueue[0x18];      /* OSMesgQueue */
    /* 0x090 */ u8 pad90[0x2E0 - 0x90];
    /* 0x2E0 */ Client *clients;
    /* 0x2E4 */ u8 pad2E4[4];
    /* 0x2E8 */ ScTask *gfxHead;
    /* 0x2EC */ u8 pad2EC[4];
    /* 0x2F0 */ ScTask *gfxTail;
    /* 0x2F4 */ ScTask *curRSPTask;
    /* 0x2F8 */ ScTask *curRDPTask;
    /* 0x2FC */ s32 frameCount;
    /* 0x300 */ s32 doAudio;
} Sched;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ s32 framebuffer;
    /* 0x114 */ u8 pad114[0x124 - 0x114];
    /* 0x124 */ u32 minRetraces;
} GfxTask;

extern s32 gDecompressorBoostRetraces;          /* retraces before the thread priority is restored */
extern u8 gDecompressor[];         /* OSThread */
extern s32 gThreadPriorityDecompressorLow;          /* its priority */
extern s32 gCurrentTick;          /* retraces since the last swap */
extern s32 gNumTicksPending;          /* swap pending */
extern s32 gPendingFB;          /* framebuffer just swapped in */
extern s32 gCurrentFB;          /* framebuffer on screen */
extern s32 gNumTicksWaiting;          /* frames waiting to be shown */
extern GfxTask *gWaitingFrames[];
extern s32 sSchedGfxWatchdog;          /* retrace counter */

extern s32 osRecvMesg(void *queue, void *msg, s32 flag);
extern s32 osSendMesg(void *queue, void *msg, s32 flag);
extern void osSetThreadPri(void *thread, s32 pri);   /* osSetThreadPri */
extern void osViSwapBuffer(s32 framebuffer);         /* osViSwapBuffer */
extern void __scAppendList(Sched *sc, ScTask *task);
extern void __scHandleRSP(Sched *sc);
extern void __scYield(Sched *sc);
extern s32 __scSchedule(Sched *sc, ScTask **sp, ScTask **dp, s32 state);
extern void __scExec(Sched *sc, ScTask *sp, ScTask *dp);
extern void Controller_Service(void);

/* Retrace: queue new tasks, swap the framebuffer when due, start the next tasks, wake the clients. */
void __scHandleRetrace(Sched *sc) {
    ScTask *rspTask;
    ScTask *sp;
    ScTask *dp;
    ScTask *task;
    ScTask *next;
    Client *client;
    GfxTask *frame;
    s32 state;
    s32 fb;

    rspTask = NULL;
    sp = NULL;
    dp = NULL;
    if (gDecompressorBoostRetraces != 0) {
        gDecompressorBoostRetraces = gDecompressorBoostRetraces - 1;
        if (gDecompressorBoostRetraces == 0) {
            osSetThreadPri(gDecompressor, gThreadPriorityDecompressorLow);
        }
    }
    sc->frameCount = sc->frameCount + 1;
    while (osRecvMesg(sc->cmdQueue, &rspTask, 0) != -1) {
        __scAppendList(sc, rspTask);
    }
    gCurrentTick = gCurrentTick + 1;
    if (gNumTicksPending != 0) {
        gCurrentTick = 0;
        gNumTicksPending = 0;
        fb = gPendingFB;
        gPendingFB = 0;
        gCurrentFB = fb;
    }
    if (gNumTicksWaiting != 0) {
        frame = gWaitingFrames[gNextFrameToShow];
        if (!((u32)(gCurrentTick + 1) < frame->minRetraces)) {
            gPendingFB = frame->framebuffer;
            osViSwapBuffer(frame->framebuffer);
            gNumTicksWaiting = gNumTicksWaiting - 1;
            gNumTicksPending = gNumTicksPending + 1;
            gNextFrameToShow = gNextFrameToShow + 1;
            if (!(gNextFrameToShow < gTotalFramebuffers)) {
                gNextFrameToShow = 0;
            }
        }
    }
    if (sc->curRSPTask == NULL && sc->curRDPTask == NULL) {
        task = sc->gfxHead;
        if (task != NULL && task->type == 1 && task->state == 0x32) {
            task->state = 2;
            sc->curRSPTask = sc->gfxHead;
            next = sc->gfxHead->next;
            sc->gfxHead = next;
            if (next == NULL) {
                sc->gfxTail = NULL;
            }
            __scHandleRSP(sc);
        }
    }
    sSchedGfxWatchdog = sSchedGfxWatchdog + 1;
    if (sSchedGfxWatchdog == 1200) {
        *(u8 *)0 = 0;
    }
    if (sc->doAudio != 0 && sc->curRSPTask != NULL) {
        __scYield(sc);
    } else {
        state = (sc->curRSPTask == NULL) << 1 | (sc->curRDPTask == NULL);
        if (__scSchedule(sc, &sp, &dp, state) != state) {
            __scExec(sc, sp, dp);
        }
    }
    client = sc->clients;
    while (client != NULL) {
        osSendMesg(client->queue, sc, 0);
        client = client->next;
    }
    Controller_Service();
}
