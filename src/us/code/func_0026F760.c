/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002568A0.c, func_002568A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void osSetEventMesg(OSEvent e, OSMesgQueue *mq, OSMesg msg);
void osStartThread(OSThread *thread);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
void *memset(void *s, int c, u32 n);
void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msgBuf, s32 count);
s32 osAiSetFrequency(u32 freq);
extern s8 D_800F2790[];
extern s32 D_800F2838;
extern s32 D_800F283C;
extern s32 D_800F2840;
extern s32 D_800F279C;
extern B12DbEnt2 D_800F22C8[];
extern void *D_800F27C0[];
extern B12AiTask *D_800F27C8[];
extern u8 D_800F27D4[];
extern OSMesgQueue D_800F2820;
extern OSMesgQueue D_800F27A0;
extern OSMesgQueue D_800F2778;
extern OSMesg D_800F2070[];
extern OSMesg D_800F2048[];
extern OSMesg D_800F2848[];
extern OSThread D_800F2098;
extern u8 D_800C5400[];
extern u8 D_800BC330[];
extern u8 D_800BC400[];
extern u8 D_800BE920[];
extern u8 D_800BFF50[];
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
    D_800F2790[0] = 0;
    cfg->dmaproc = (void *)__amDmaNew;
    freq = osAiSetFrequency(c2->unk0);
    cfg->outputRate = freq;
    t = (f32)c2->unk4 * (f32)freq * 0.016666668f;
    n = (s32)t;
    D_800F2838 = n;
    if ((f32)n < t) {
        D_800F2838 = n + 1;
    }
    if (D_800F2838 & 0xF) {
        D_800F2838 = (D_800F2838 & ~0xF) + 0x10;
    }
    alInit(D_800F27D4, cfg);
    D_800F22C8[0].link.prev = 0;
    D_800F22C8[0].link.next = 0;
    for (i = 0; i < 0x3B; i++) {
        ALLink *next = &D_800F22C8[i + 1].link;
        ALLink *prev = &D_800F22C8[i].link;
        alLink(next, prev);
        D_800F22C8[i].data = alHeapDBAlloc(0, 0, cfg->heap, 1, 0x300);
    }
    D_800F22C8[i].data = alHeapDBAlloc(0, 0, cfg->heap, 1, 0x300);
    for (i = 0; i < 2; i++) {
        D_800F27C0[i] = alHeapDBAlloc(0, 0, cfg->heap, 1, c2->unk8 * 8);
    }
    D_800F283C = c2->unk8;
    i = 0;
    a20 = D_800BC330;
    a24 = D_800BC400 - D_800BC330;
    tp = D_800F27C8;
    for (; i < 3; i++) {
        tk = alHeapDBAlloc(0, 0, cfg->heap, 1, 0x70);
        n4 = D_800F2838 * 4;
        *tp = tk;
        (*tp)->cmd = alHeapDBAlloc(0, 0, cfg->heap, 1, n4);
        (*tp)->f58 = &D_800F2820;
        (*tp)->f5C = *tp;
        (*tp)->f18 = 2;
        (*tp)->f20 = a20;
        (*tp)->f24 = a24;
        (*tp)->f1C = 0;
        (*tp)->f28 = D_800BE920;
        (*tp)->f2C = 0x1000;
        (*tp)->f30 = D_800BFF50;
        (*tp)->f34 = 0x800;
        (*tp)->f50 = 0;
        (*tp)->f54 = 0;
        (*tp)->f38 = 0;
        (*tp)->f3C = 0;
        (*tp)->f40 = 0;
        (*tp)->f44 = 0;
        tp++;
    }
    osCreateMesgQueue(&D_800F2820, D_800F2070, 10);
    osCreateMesgQueue(&D_800F27A0, D_800F2048, 10);
    osCreateMesgQueue(&D_800F2778, D_800F2848, 0x3C);
    osSetEventMesg(6, &D_800F27A0, (OSMesg)7);
    D_800F279C = 0;
    D_800F2840 = (s32)cfg->heap;
    memset(D_800C5400, 8, 0x2800);
    osCreateThread(&D_800F2098, 8, __amMain, 0, D_800C5400 + 0x2800, pri);
    osStartThread(&D_800F2098);
    osSendMesg(&D_800F27A0, (OSMesg)7, 0);
}
