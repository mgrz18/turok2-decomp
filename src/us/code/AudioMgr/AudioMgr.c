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
s32 *__amDmaNew(AMDMAState **arg0);
void __amMain(void);
void alInit(void *g, ALSynConfig *c);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: amCreateAudioMgr) */
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
#endif

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00256CB0.c, func_00256CB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"

u32 osAiGetStatus(void);
void osScAddClient(void *a, void *b, void *c);
s32 __amHandleFrameMsg(AMAudioInfo *arg0);
void alClose(void *g);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: __amMain) */
void __amMain(void) {
    void *sp10[2];
    u32 msg;
    u32 cnt;
    s32 arg;
    s32 done;
    OSTime last;
    OSTime now;
    AMCacheEntry *p;
    cnt = arg = done = 0;
    last = 0;
    osScAddClient(&gScheduler, sp10, &gAudioFrameMsgQ);
    do {
        now = osGetTime();
        sAmBusyTime = (u32)(((now - last) << 6) / 3000);
        osRecvMesg(&gAudioFrameMsgQ, (OSMesg *)&msg, 1);
        last = osGetTime();
        switch (msg) {
        case 4:
            break;
        case 1:
            cnt++;
            if (cnt >= 0x15) {
                cnt = 0;
        case 7:
                sAmFramesHandled++;
                while (!(osAiGetStatus() & 0x80000000)) {
                    /* The ROM passes a second argument the definition does not take: called through a type of its own. */
                    ((void (*)(AMAudioInfo *, s32))__amHandleFrameMsg)(gAudioInfo[audFrameCt % 3], arg);
                }
                for (p = gAudioDmaCache; p; p = p->next) {
                    if (p->state) {
                        p->state--;
                    }
                }
            }
            break;
        }
    } while (done == 0);
    alClose(&gAudioGlobals);
}
#endif
u32 __amDMA(s32 offset, s32 length);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: __amDmaNew) */
s32 *__amDmaNew(AMDMAState **arg0) {
    if (gAudioDmaState.initialized == 0) {
        gAudioDmaState.firstFree = gAudioDmaBuffers;
        gAudioDmaState.firstUsed = NULL;
        gAudioDmaState.initialized = 1U;
    }
    *arg0 = &gAudioDmaState;
    return &__amDMA;
}
#endif

sptr alAudioFrame(sptr, sptr, sptr, s32);
void __clearAudioDMA(void);
s32 Sound__Update();                            /* extern */
s32 Sound__Lock();                            /* extern */
s32 Sound__Unlock();                            /* extern */
s32 Sched_SubmitTask(s32 *, void *);           /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: __amHandleFrameMsg) */
s32 __amHandleFrameMsg(AMAudioInfo *arg0) {
    s32 sp10;
    s32 sp14;
    s32 temp_s0;
    sptr temp_s0_2;
    s32 temp_v0;

    __clearAudioDMA();
    temp_s0 = osVirtualToPhysical(arg0->data);
    arg0->frameSamples = gAudioFrameSize;
    Sound__Lock();
    temp_s0_2 = alAudioFrame((sptr) gAudioCmdLists[curAcmdList], (sptr) &sp10, temp_s0, (s32) (s16) arg0->frameSamples);
    Sound__Unlock();
    if (sp10 == 0) {
        return 0;
    }
    temp_v0 = curAcmdList * 4;
    arg0->task.list.t.data_ptr = (u64 *) *(Acmd **) ((u8 *) gAudioCmdLists + temp_v0);
    arg0->task.list.t.data_size = (((s32) (temp_s0_2 - (sptr) *(Acmd **) ((u8 *) gAudioCmdLists + temp_v0))) >> 3) * 8;
    osWritebackDCacheAll();
    Sched_SubmitTask(&gScheduler, &arg0->task);
    curAcmdList ^= 1;
    Sound__Update();
    osRecvMesg(&gAudioReplyMsgQ, &sp14, 1);
    osAiSetNextBuffer(arg0->data, (s16) arg0->frameSamples * 4);
    return 1;
}
#endif

/* Waits for this frame's audio DMAs and frees the buffers not used lately. */
void __clearAudioDMA(void) {
    s32 sp10;
    u32 var_s0;
    AMDMABuffer *temp_s1;
    AMDMABuffer *var_s0_2;

    var_s0 = 0;
    sp10 = 0;
    if (nextDMA != 0) {
        do {
            osRecvMesg(&gAudioDmaMessageQ, &sp10, 1);
            var_s0 += 1;
        } while (var_s0 < (u32) nextDMA);
    }
    var_s0_2 = gAudioDmaState.firstUsed;
    if (var_s0_2 != NULL) {
        do {
            temp_s1 = (AMDMABuffer *) var_s0_2->node.next;
            if ((u32) (var_s0_2->lastFrame + 1) < (u32) audFrameCt) {
                if (gAudioDmaState.firstUsed == var_s0_2) {
                    gAudioDmaState.firstUsed = temp_s1;
                }
                alUnlink((ALLink *) var_s0_2);
                if (gAudioDmaState.firstFree == NULL) {
                    gAudioDmaState.firstFree = var_s0_2;
                    var_s0_2->node.next = NULL;
                    var_s0_2->node.prev = 0;
                    goto block_11;
                }
                alLink((ALLink *) var_s0_2, (ALLink *) gAudioDmaState.firstFree);
                var_s0_2 = temp_s1;
            } else {
block_11:
                var_s0_2 = temp_s1;
            }
        } while (var_s0_2 != NULL);
    }
    nextDMA = 0;
    audFrameCt += 1;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_14.c, func_802570C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *func_0029E460(void *destination, const void *source, int count);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: AudioDMA_CacheBlock) */
u32 AudioDMA_CacheBlock(void *block, s32 size) {
    AMCacheEntry *entry;
    AMCacheEntry *unused;
    unused = 0;
    for (entry = gAudioDmaCache; entry != 0; entry = entry->next) {
        if (entry->key == block) {
            entry->state = 3;
            if (entry->size != size) {
                entry->size = size;
                func_0029E460(entry->data, block, size);
                osWritebackDCache(entry->data, size);
            }
            return osVirtualToPhysical(entry->data);
        }
        if (entry->state == 0) {
            unused = entry;
        }
    }
    if (unused == 0) {
        unused = (AMCacheEntry *)alHeapDBAlloc(0, 0, gAudioHeap, 1, 0x90);
        unused->next = gAudioDmaCache;
        gAudioDmaCache = unused;
        sAudioDMACacheBlocks++;
    }
    unused->key = block;
    unused->state = 3;
    unused->size = size;
    func_0029E460(unused->data, block, size);
    return osVirtualToPhysical(unused->data);
}
#endif

#include "globals/sysrc.h"

extern void CLoader__LoadAsync(CLoader *reader, u32, s32, sptr, void *, s32, s32);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: __amDMA) */
u32 __amDMA(s32 offset, s32 length) {
    AMDMABuffer *window;
    AMDMABuffer *after;
    AMDMABuffer **pool;
    u32 address;
    s32 end;
    s32 top;
    s32 odd;
    s32 word;
    after = 0;
    address = offset + gAudioRomBase;
    end = address + length;
    for (window = gAudioDmaState_firstUsed; window != 0; window = (AMDMABuffer *) window->node.next) {
        top = window->startAddr + 0x300;
        word = window->startAddr;
        if (address < word) {
            break;
        }
        after = window;
        if (top >= end) {
            window->lastFrame = audFrameCt;
            return osVirtualToPhysical((void *) (window->ptr + address - window->startAddr));
        }
    }
    pool = &gAudioDmaState_firstFree;
    window = *pool;
    *pool = (AMDMABuffer *) window->node.next;
    alUnlink((ALLink *) window);
    if (after != 0) {
        alLink((ALLink *) window, (ALLink *) after);
    } else if (pool[-1] != 0) {
        after = pool[-1];
        pool[-1] = window;
        window->node.next = (ALLink *) after;
        window->node.prev = 0;
        after->node.prev = (ALLink *) window;
    } else {
        pool[-1] = window;
        window->node.next = 0;
        window->node.prev = 0;
    }
    odd = address & 1;
    address -= odd;
    window->startAddr = address;
    window->lastFrame = audFrameCt;
    word = (sptr) window->ptr;
    nextDMA++;
    CLoader__LoadAsync(&gLoader, address, 0x300, word, &gAudioDmaMessageQ, 0, 1);
    return osVirtualToPhysical((void *) word) + odd;
}
#endif
