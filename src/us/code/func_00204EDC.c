/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002533DC.c, func_002533DC.
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
u32 func_0029DFF0(void);
void func_0029E010(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
typedef struct B12Node B12Node;
struct B12Node {
               u32 key;
               s32 unk4;
               s32 refs;
               u32 flags;
               u32 unk10;
               B12Node *link;
               B12Node *link18;
               u8 pad1C[4];
               B12Node *prev;
               B12Node *next;
};
extern OSMesgQueue D_800D8DB0;
extern s32 D_800D8DCC;
B12Node *func_002060E0(s32 a, s32 b, u32 c, s32 d);
B12Node *func_00204EDC(s32 unused, s32 a1, u32 a2, s32 a3) {
    u32 mask;
    u32 m2;
    B12Node *r;
    mask = func_0029DFF0();
    if (++D_800D8DCC != 1) {
        func_0029E010(mask);
        osRecvMesg(&D_800D8DB0, 0, 1);
    } else {
        func_0029E010(mask);
    }
    r = func_002060E0(0, a1, a2, a3);
    m2 = func_0029DFF0();
    if (--D_800D8DCC != 0) {
        func_0029E010(m2);
        osSendMesg(&D_800D8DB0, 0, 1);
    } else {
        func_0029E010(m2);
    }
    return r;
}
