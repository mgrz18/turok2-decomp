/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002568A0.c, func_002568A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "globals/audio.h"
#include "globals/audiomgr.h"
#include "globals/ucode.h"
#include "globals/frame.h"
#include "protos/ultra.h"
#include "protos/libaudio.h"

void *memset(void *s, int c, uptr n);
void __amDmaNew(void);
void __amMain(void *arg);
void alInit(void *g, ALSynConfig *c);
void amCreateAudioMgr(ALSynConfig *cfg, s32 pri, B12AiCfg *c2) {
    s32 freq;
    u32 n;
    f32 t;
    u32 i;
    AMAudioInfo **tp;
    AMAudioInfo *tk;
    u8 *a20;
    s32 a24;
    s32 n4;
    gAudioDmaState.initialized = 0;
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
    gAudioDmaBuffers[0].node.prev = 0;
    gAudioDmaBuffers[0].node.next = 0;
    for (i = 0; i < 0x3B; i++) {
        ALLink *next = &gAudioDmaBuffers[i + 1].node;
        ALLink *prev = &gAudioDmaBuffers[i].node;
        alLink((ALLink *) next, (ALLink *) prev);
        gAudioDmaBuffers[i].ptr = alHeapDBAlloc(0, 0, cfg->heap, 1, 0x300);
    }
    gAudioDmaBuffers[i].ptr = alHeapDBAlloc(0, 0, cfg->heap, 1, 0x300);
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
        (*tp)->data = alHeapDBAlloc(0, 0, cfg->heap, 1, n4);
        (*tp)->task.msgQ = &gAudioReplyMsgQ;
        (*tp)->task.msg = *tp;
        (*tp)->task.list.t.type = 2;
        (*tp)->task.list.t.ucode_boot = a20;
        (*tp)->task.list.t.ucode_boot_size = a24;
        (*tp)->task.list.t.flags = 0;
        (*tp)->task.list.t.ucode = aspMainTextStart;
        (*tp)->task.list.t.ucode_size = 0x1000;
        (*tp)->task.list.t.ucode_data = aspMainDataStart;
        (*tp)->task.list.t.ucode_data_size = 0x800;
        (*tp)->task.list.t.yield_data_ptr = 0;
        (*tp)->task.list.t.yield_data_size = 0;
        (*tp)->task.list.t.dram_stack = 0;
        (*tp)->task.list.t.dram_stack_size = 0;
        (*tp)->task.list.t.output_buff = 0;
        (*tp)->task.list.t.output_buff_size = 0;
        tp++;
    }
    osCreateMesgQueue(&gAudioReplyMsgQ, gAudioReplyMsgBuf, 10);
    osCreateMesgQueue(&gAudioFrameMsgQ, gAudioFrameMsgBuf, 10);
    osCreateMesgQueue(&gAudioDmaMessageQ, gAudioDmaMessageBuf, 0x3C);
    osSetEventMesg(6, &gAudioFrameMsgQ, (OSMesg)7);
    gAudioDmaCache = NULL;
    gAudioHeap = (sptr)cfg->heap;
    memset(gThreadStackAudio, 8, 0x2800);
    osCreateThread(&gAudioThread, 8, __amMain, 0, gThreadStackAudio + 0x2800, pri);
    osStartThread(&gAudioThread);
    osSendMesg(&gAudioFrameMsgQ, (OSMesg)7, 0);
}
