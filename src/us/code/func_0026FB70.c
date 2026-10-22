/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00256CB0.c, func_00256CB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;
typedef float f32;
typedef void *OSMesg;
typedef u64 OSTime;
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
OSTime func_0029B170(void);
u32 osAiGetStatus(void);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern u32 D_800B2660;
extern s32 D_800B266C;
extern u32 D_800B2670;
extern u8 D_8011AD90[];
extern OSMesgQueue D_800F27A0;
extern s32 D_800F27C8[];
extern u8 D_800F27D4[];
typedef struct B12Timer2 {
    struct B12Timer2 *next;
    u8 pad4[4];
    u16 count;
} B12Timer2;
extern B12Timer2 *D_800F279C;
void func_0028AF00(void *a, void *b, void *c);
void func_0026FD88(s32 buf, s32 z);
void func_00293430(void *g);
void func_0026FB70(void) {
    s32 sp10[2];
    u32 msg;
    u32 cnt;
    s32 arg;
    s32 done;
    OSTime last;
    OSTime now;
    B12Timer2 *p;
    cnt = arg = done = 0;
    last = 0;
    func_0028AF00(D_8011AD90, sp10, &D_800F27A0);
    do {
        now = func_0029B170();
        D_800B2670 = (u32)(((now - last) << 6) / 3000);
        osRecvMesg(&D_800F27A0, (OSMesg *)&msg, 1);
        last = func_0029B170();
        switch (msg) {
        case 4:
            break;
        case 1:
            cnt++;
            if (cnt >= 0x15) {
                cnt = 0;
        case 7:
                D_800B266C++;
                while (!(osAiGetStatus() & 0x80000000)) {
                    func_0026FD88(D_800F27C8[D_800B2660 % 3], arg);
                }
                for (p = D_800F279C; p; p = p->next) {
                    if (p->count) {
                        p->count--;
                    }
                }
            }
            break;
        }
    } while (done == 0);
    func_00293430(D_800F27D4);
}
