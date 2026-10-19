/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_25.c, func_802BB5F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
struct OSThread_s;
typedef struct OSThread_s OSThread_s;
struct OSThreadContext;
typedef struct OSThreadContext OSThreadContext;
struct OSThreadContext;
struct OSThreadContext {
    u64 at;
    u64 v0;
    u64 v1;
    u64 a0;
    u64 a1;
    u64 a2;
    u64 a3;
    u64 t0;
    u64 t1;
    u64 t2;
    u64 t3;
    u64 t4;
    u64 t5;
    u64 t6;
    u64 t7;
    u64 s0;
    u64 s1;
    u64 s2;
    u64 s3;
    u64 s4;
    u64 s5;
    u64 s6;
    u64 s7;
    u64 t8;
    u64 t9;
    u64 gp;
    u64 sp;
    u64 s8;
    u64 ra;
    u64 lo;
    u64 hi;
    u32 sr;
    u32 pc;
    u32 cause;
    u32 badvaddr;
    u32 rcp;
    u32 fpcsr;
};
struct OSThread_s;
struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    s32 id;
    int fp;
    OSThreadContext context;
};
extern OSThread_s *D_800B8918;
extern OSThread_s *D_800B8920;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 mask);
extern void __osDequeueThread(OSThread_s **queue, OSThread_s *t);
extern void func_0029C878(OSThread_s **queue, OSThread_s *t);
extern void func_0029C6DC(OSThread_s **queue);
void func_0029B9B0(OSThread_s *t, s32 pri)
{
    register u32 saveMask;
    OSThread_s **runQueue;
    saveMask = func_0029DFF0();
    if (t == 0) {
        t = D_800B8920;
    }
    if (t->priority != pri) {
        t->priority = pri;
        if (t != D_800B8920 && t->state != 1) {
            __osDequeueThread(t->queue, t);
            func_0029C878(t->queue, t);
        }
        runQueue = &D_800B8918;
        if (D_800B8920->priority < (*runQueue)->priority) {
            D_800B8920->state = 2;
            func_0029C6DC(runQueue);
        }
    }
    func_0029E010(saveMask);
}
