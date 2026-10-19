/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002568A0.c, func_002568A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;
typedef float f32;
typedef void *OSMesg;
typedef s32 OSPri;
typedef s32 OSId;
typedef union {
    struct { f32 f_odd; f32 f_even; } f;
} __OSfp;
typedef struct {
               u64 at, v0, v1, a0, a1, a2, a3;
               u64 t0, t1, t2, t3, t4, t5, t6, t7;
               u64 s0, s1, s2, s3, s4, s5, s6, s7;
               u64 t8, t9, gp, sp, s8, ra;
                u64 lo, hi;
                u32 sr, pc, cause, badvaddr, rcp;
                u32 fpcsr;
    __OSfp fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
    __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;
typedef struct {
    u32 flag;
    u32 count;
    u64 time;
} __OSThreadprofile_s;
typedef struct OSThread_s {
               struct OSThread_s *next;
               OSPri priority;
               struct OSThread_s **queue;
               struct OSThread_s *tlnext;
               u16 state;
               u16 flags;
               OSId id;
               int fp;
               __OSThreadprofile_s *thprof;
               __OSThreadContext context;
} OSThread;
typedef struct OSMesgQueue_s {
               OSThread *mtqueue;
               OSThread *fullqueue;
               s32 validCount;
               s32 first;
               s32 msgCount;
               OSMesg *msg;
} OSMesgQueue;
typedef u32 OSEvent;
void func_0029B950(OSEvent e, OSMesgQueue *mq, OSMesg msg);
void func_0029BB10(OSThread *thread);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
void *func_00266C5C(void *s, int c, u32 n);
void func_0029B060(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void func_0029B030(OSMesgQueue *mq, OSMesg *msgBuf, s32 count);
s32 func_002984A0(u32 freq);
typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;
typedef struct {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    void *dmaproc;
    void *heap;
    s32 outputRate;
    u8 fxType;
} ALSynConfig;
typedef struct B12AiCfg {
    u32 unk0;
    u32 unk4;
    s32 unk8;
} B12AiCfg;
typedef struct B12AiTask {
               s32 *cmd;
               u8 pad4[0x14];
               s32 f18;
               s32 f1C;
               u8 *f20;
               s32 f24;
               u8 *f28;
               s32 f2C;
               u8 *f30;
               s32 f34;
               s32 f38;
               s32 f3C;
               s32 f40;
               s32 f44;
               u8 pad48[8];
               s32 f50;
               s32 f54;
               OSMesgQueue *f58;
               struct B12AiTask *f5C;
               u8 pad60[0x10];
} B12AiTask;
typedef struct B12DbEnt2 {
    ALLink link;
    s32 f8;
    s32 fC;
    void *data;
} B12DbEnt2;
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
void D_0026FD4C(void);
void D_0026FB70(void *arg);
void func_00293400(void *g, ALSynConfig *c);
void *func_002912A0(u8 *file, s32 line, void *hp, s32 num, s32 size);
void func_0026F760(ALSynConfig *cfg, s32 pri, B12AiCfg *c2) {
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
    cfg->dmaproc = (void *)D_0026FD4C;
    freq = func_002984A0(c2->unk0);
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
    func_00293400(D_800F27D4, cfg);
    D_800F22C8[0].link.prev = 0;
    D_800F22C8[0].link.next = 0;
    for (i = 0; i < 0x3B; i++) {
        ALLink *next = &D_800F22C8[i + 1].link;
        ALLink *prev = &D_800F22C8[i].link;
        func_002933E0(next, prev);
        D_800F22C8[i].data = func_002912A0(0, 0, cfg->heap, 1, 0x300);
    }
    D_800F22C8[i].data = func_002912A0(0, 0, cfg->heap, 1, 0x300);
    for (i = 0; i < 2; i++) {
        D_800F27C0[i] = func_002912A0(0, 0, cfg->heap, 1, c2->unk8 * 8);
    }
    D_800F283C = c2->unk8;
    i = 0;
    a20 = D_800BC330;
    a24 = D_800BC400 - D_800BC330;
    tp = D_800F27C8;
    for (; i < 3; i++) {
        tk = func_002912A0(0, 0, cfg->heap, 1, 0x70);
        n4 = D_800F2838 * 4;
        *tp = tk;
        (*tp)->cmd = func_002912A0(0, 0, cfg->heap, 1, n4);
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
    func_0029B030(&D_800F2820, D_800F2070, 10);
    func_0029B030(&D_800F27A0, D_800F2048, 10);
    func_0029B030(&D_800F2778, D_800F2848, 0x3C);
    func_0029B950(6, &D_800F27A0, (OSMesg)7);
    D_800F279C = 0;
    D_800F2840 = (s32)cfg->heap;
    func_00266C5C(D_800C5400, 8, 0x2800);
    func_0029B060(&D_800F2098, 8, D_0026FB70, 0, D_800C5400 + 0x2800, pri);
    func_0029BB10(&D_800F2098);
    osSendMesg(&D_800F27A0, (OSMesg)7, 0);
}
