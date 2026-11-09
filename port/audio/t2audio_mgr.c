/* t2audio_mgr.c: the audio manager for the host (docs/port/AUDIO.md §3).
 *
 * The game's audio manager is the SDK's audiomgr.c pattern (func_0026F760 amCreateAudioMgr,
 * func_0026FB70 __amMain, func_0026FD88 __amHandleFrameMsg, func_0026FD4C __amDmaNew,
 * func_002700D0 __amDMA). On the host those five files are replaced (port/cmake/link.cmake):
 *   - their task records, DMA windows and frame records are m2c views with N64 offsets,
 *     and the RSP task they build (an OSTask with 32-bit pointers) goes through the game's
 *     scheduler only to reach the RSP; here the command list is run by the ABI1 interpreter
 *     (t2asp.c) directly on the audio thread, and the frame goes to the AI queue;
 *   - __amDMA copies wave data through 0x300-byte windows with the loader thread
 *     (CLoader__LoadAsync); here the wave table region is resident (a copy of the ROM from
 *     gAudioRomBase on), so the DMA callback returns the address of the bytes themselves.
 * What stays the game's: alInit (synthesizer), alAudioFrame and every filter that builds the
 * command list, the sound and sequence players, CAudioWorld, Sound__Lock/Unlock/Update.
 *
 * Timing: one synthesizer frame per VI retrace (osSetTimer with the retrace interval on the
 * virtual clock), gAudioFrameSize samples each (22050 Hz / 60, rounded up to 16: 368). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "ultra_types.h"
#include "structs/libaudio.h"
#include "protos/ultra.h"
#include "t2port.h"

#ifndef OS_MESG_BLOCK
#define OS_MESG_BLOCK 1
#endif
#include "t2audio.h"

/* game symbols */
extern ALGlobals gAudioGlobals;
extern s32 gAudioFrameSize;
extern s32 gAudioMaxRSPCmds;
extern s32 gAudioRomBase;
extern u32 audFrameCt;
void alInit(ALGlobals *g, ALSynConfig *c);
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
Acmd *alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen);
void Sound__Lock(void);
void Sound__Unlock(void);
void Sound__Update(void);

#define T2_COUNT_PER_RETRACE 781250ull /* 46.875 MHz / 60 (t2ultra.c's virtual clock) */

typedef struct {
    s32 rate;
    s32 framesPerField;
    s32 maxACMDSize;
} AmConfig; /* CAudioWorld__Initialize's AiCfg */

static int sEnabled;
static OSThread sThread;
static OSMesgQueue sFrameQ;
static OSMesg sFrameMsg[8];
static u8 sTimerKey[0x40]; /* an OSTimer-sized key (t2ultra uses its address only) */
static Acmd *sCmd[2];
static s16 *sOut[2];
static s32 sMaxCmds;
static int sCur;
static u32 sRate = 22050;

static const u8 *sTbl; /* the ROM from gAudioRomBase on (resident, below 2 GB) */
static size_t sTblSize;

static T2AudioStats sStats;

void T2Audio_Enable(int on) {
    sEnabled = on;
}
int T2Audio_Enabled(void) {
    return sEnabled;
}
uint32_t T2Audio_Rate(void) {
    return sRate;
}
void T2Audio_GetStats(T2AudioStats *out) {
    *out = sStats;
}

/* ---- the AI queue: interleaved stereo s16 ---- */

#define AIQ_FRAMES 16384
static int16_t sAiq[AIQ_FRAMES * 2];
static size_t sAiqHead, sAiqCount;

static void aiq_push(const s16 *buf, size_t frames) {
    size_t i;
    for (i = 0; i < frames; i++) {
        size_t at;
        int l = buf[2 * i], r = buf[2 * i + 1];
        if (l < 0 ? -l > sStats.peak : l > sStats.peak)
            sStats.peak = l < 0 ? -l : l;
        if (r < 0 ? -r > sStats.peak : r > sStats.peak)
            sStats.peak = r < 0 ? -r : r;
        if (sAiqCount == AIQ_FRAMES) { /* full: drop the oldest */
            sAiqHead = (sAiqHead + 1) % AIQ_FRAMES;
            sAiqCount--;
            sStats.dropped++;
        }
        at = (sAiqHead + sAiqCount) % AIQ_FRAMES;
        sAiq[2 * at] = (int16_t)l;
        sAiq[2 * at + 1] = (int16_t)r;
        sAiqCount++;
    }
    sStats.samples += frames;
}

size_t T2Audio_TakeSamples(int16_t *dst, size_t max) {
    size_t n = 0;
    while (n < max && sAiqCount > 0) {
        dst[2 * n] = sAiq[2 * sAiqHead];
        dst[2 * n + 1] = sAiq[2 * sAiqHead + 1];
        sAiqHead = (sAiqHead + 1) % AIQ_FRAMES;
        sAiqCount--;
        n++;
    }
    return n;
}
size_t T2Audio_Queued(void) {
    return sAiqCount;
}

/* ---- DMA (func_0026FD4C __amDmaNew, func_002700D0 __amDMA) ---- */

const uint8_t *T2Audio_TblBytes(uint32_t romAddr, uint32_t len) {
    u32 base = (u32)gAudioRomBase;
    if (sTbl == NULL || romAddr < base || romAddr - base > sTblSize || len > sTblSize - (romAddr - base))
        return NULL;
    return sTbl + (romAddr - base);
}

/* ALDMAproc: the wave table bytes at gAudioRomBase + offset, as the 32-bit address the command
 * list carries (osVirtualToPhysical). */
static s32 am_dma(s32 offset, s32 length, void *state) {
    const u8 *p;
    (void)state;
    sStats.dmaCalls++;
    p = T2Audio_TblBytes((u32)gAudioRomBase + (u32)offset, (u32)length);
    if (p == NULL) {
        t2_log_once("__amDMA: wave data outside the ROM");
        p = sTbl;
    }
    return (s32)osVirtualToPhysical((void *)p);
}

ALDMAproc __amDmaNew(void **state) {
    *state = NULL;
    return am_dma;
}
s32 __amDMA(s32 offset, s32 length, void *state) {
    return am_dma(offset, length, state);
}

static void load_tbl(void) {
    size_t rs = 0;
    const u8 *rom = t2_rom(&rs);
    u32 base = (u32)gAudioRomBase;
    u8 *p;
    if (rom == NULL || base >= rs) {
        t2_log_once("t2audio: no ROM image: no wave data");
        return;
    }
    sTblSize = rs - base;
    p = t2_lowmem_alloc(sTblSize + 16);
    memcpy(p, rom + base, sTblSize);
    sTbl = p;
}

/* ---- the manager ---- */

static void am_frame(void) {
    s32 cmdLen = 0;
    Acmd *end;
    s16 *out = sOut[sCur];
    s32 n = gAudioFrameSize;
    if (sTbl == NULL)
        load_tbl();
    memset(out, 0, (size_t)n * 4);
    Sound__Lock();
    end = alAudioFrame(sCmd[sCur], &cmdLen, out, n);
    Sound__Unlock();
    if (end != NULL && end > sCmd[sCur])
        cmdLen = (s32)(end - sCmd[sCur]);
    if (cmdLen > sMaxCmds) {
        fprintf(stderr, "t2audio: command list overflow (%d > %d)\n", (int)cmdLen, (int)sMaxCmds);
        abort();
    }
    if (cmdLen > 0)
        T2Asp_Run((const uint32_t *)sCmd[sCur], (size_t)cmdLen);
    aiq_push(out, (size_t)n);
    sStats.frames++;
    audFrameCt++;
    sCur ^= 1;
    Sound__Update();
}

static void am_main(void *arg) {
    OSMesg msg;
    (void)arg;
    for (;;) {
        osRecvMesg(&sFrameQ, &msg, OS_MESG_BLOCK);
        am_frame();
    }
}

/* func_0026F760 */
void amCreateAudioMgr(ALSynConfig *cfg, s32 pri, AmConfig *ac) {
    f32 t;
    s32 n;
    int i;
    cfg->dmaproc = (void *)__amDmaNew;
    sRate = (u32)osAiSetFrequency((u32)ac->rate);
    cfg->outputRate = (s32)sRate;
    t = (f32)ac->framesPerField * (f32)sRate / 60.0f;
    n = (s32)t;
    if ((f32)n < t)
        n++;
    if (n & 0xF)
        n = (n & ~0xF) + 0x10;
    gAudioFrameSize = n;
    alInit(&gAudioGlobals, cfg);
    sMaxCmds = ac->maxACMDSize;
    gAudioMaxRSPCmds = ac->maxACMDSize;
    for (i = 0; i < 2; i++) {
        sCmd[i] = t2_lowmem_alloc(sizeof(Acmd) * (size_t)sMaxCmds);
        sOut[i] = t2_lowmem_alloc((size_t)n * 4 + 64);
    }
    load_tbl();
    osCreateMesgQueue(&sFrameQ, sFrameMsg, 8);
    osCreateThread(&sThread, 8, am_main, NULL, NULL, pri);
    osStartThread(&sThread);
    osSetTimer((OSTimer *)sTimerKey, T2_COUNT_PER_RETRACE, T2_COUNT_PER_RETRACE, &sFrameQ, (OSMesg)7);
    fprintf(stderr, "t2audio: audio manager: %u Hz, %d samples per frame, %d commands per list\n", (unsigned)sRate,
            (int)n, (int)sMaxCmds);
}

/* func_0026FB70 / func_0026FD88: the thread and the frame handler are am_main / am_frame */
void __amMain(void *arg) {
    am_main(arg);
}
s32 __amHandleFrameMsg(void *info) {
    (void)info;
    am_frame();
    return 1;
}

/* Runs one synthesizer frame now (tests). */
void T2Audio_RunFrame(void) {
    am_frame();
}
