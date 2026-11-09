/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002568A0.c, func_002568A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "globals/audio.h"
#include "globals/ucode.h"
#include "globals/frame.h"
#include "protos/ultra.h"

void *memset(void *s, int c, uptr n);
extern s8 gAudioDmaState[];
extern s32 gAudioDmaCache;
extern B12DbEnt2 gAudioDmaBuffers[];
extern void *gAudioCmdLists[];
extern B12AiTask *gAudioInfo[];
void __amDmaNew(void);
void __amMain(void *arg);
void alInit(void *g, ALSynConfig *c);
void *alHeapDBAlloc(u8 *file, s32 line, void *hp, s32 num, s32 size);
void amCreateAudioMgr(ALSynConfig *cfg, s32 pri, B12AiCfg *c2) {
    s32 freq;
    u32 n;
    f32 t;
    u32 i;
    B12AiTask **tp;
    B12AiTask *tk;
    u8 *a20;
    s32 a24;
    s32 n4;
    gAudioDmaState[0] = 0;
    cfg->dmaproc = (void *)__amDmaNew;
    freq = osAiSetFrequency(c2->unk0);
    cfg->outputRate = freq;
    t = (f32)c2->unk4 * (f32)freq * 0.016666668f;
    n = (s32)t;
    gAudioFrameSize = n;
    if ((f32)n < t) {
        gAudioFrameSize = n + 1;
    }
    if (gAudioFrameSize & 0xF) {
        gAudioFrameSize = (gAudioFrameSize & ~0xF) + 0x10;
    }
    alInit(&gAudioGlobals, cfg);
    gAudioDmaBuffers[0].link.prev = 0;
    gAudioDmaBuffers[0].link.next = 0;
    for (i = 0; i < 0x3B; i++) {
        ALLink *next = &gAudioDmaBuffers[i + 1].link;
        ALLink *prev = &gAudioDmaBuffers[i].link;
        alLink(next, prev);
        gAudioDmaBuffers[i].data = alHeapDBAlloc(0, 0, cfg->heap, 1, 0x300);
    }
    gAudioDmaBuffers[i].data = alHeapDBAlloc(0, 0, cfg->heap, 1, 0x300);
    for (i = 0; i < 2; i++) {
        gAudioCmdLists[i] = alHeapDBAlloc(0, 0, cfg->heap, 1, c2->unk8 * 8);
    }
    gAudioMaxRSPCmds = c2->unk8;
    i = 0;
    a20 = (u8 *)rspbootTextStart;
    a24 = (u8 *)gspF3DEX_NoN_fifoTextStart - (u8 *)rspbootTextStart;
    tp = gAudioInfo;
    for (; i < 3; i++) {
        tk = alHeapDBAlloc(0, 0, cfg->heap, 1, 0x70);
        n4 = gAudioFrameSize * 4;
        *tp = tk;
        (*tp)->cmd = alHeapDBAlloc(0, 0, cfg->heap, 1, n4);
        (*tp)->f58 = &gAudioReplyMsgQ;
        (*tp)->f5C = *tp;
        (*tp)->f18 = 2;
        (*tp)->f20 = a20;
        (*tp)->f24 = a24;
        (*tp)->f1C = 0;
        (*tp)->f28 = aspMainTextStart;
        (*tp)->f2C = 0x1000;
        (*tp)->f30 = aspMainDataStart;
        (*tp)->f34 = 0x800;
        (*tp)->f50 = 0;
        (*tp)->f54 = 0;
        (*tp)->f38 = 0;
        (*tp)->f3C = 0;
        (*tp)->f40 = 0;
        (*tp)->f44 = 0;
        tp++;
    }
    osCreateMesgQueue(&gAudioReplyMsgQ, gAudioReplyMsgBuf, 10);
    osCreateMesgQueue(&gAudioFrameMsgQ, gAudioFrameMsgBuf, 10);
    osCreateMesgQueue(&gAudioDmaMessageQ, gAudioDmaMessageBuf, 0x3C);
    osSetEventMesg(6, &gAudioFrameMsgQ, (OSMesg)7);
    gAudioDmaCache = 0;
    gAudioHeap = (sptr)cfg->heap;
    memset(gThreadStackAudio, 8, 0x2800);
    osCreateThread(&gAudioThread, 8, __amMain, 0, gThreadStackAudio + 0x2800, pri);
    osStartThread(&gAudioThread);
    osSendMesg(&gAudioFrameMsgQ, (OSMesg)7, 0);
}
