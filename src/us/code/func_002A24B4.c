/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002016FC.c, func_002016FC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

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
void func_0029B060(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void func_0029B030(OSMesgQueue *mq, OSMesg *msgBuf, s32 count);
extern volatile s32 D_800BC300;
extern OSMesg D_801270B4[];
extern OSMesgQueue D_801270C0;
extern OSMesg D_801280E0[];
extern OSMesg D_801280E4[];
extern u8 D_801280DC[];
extern OSMesgQueue D_801280F0;
extern OSMesgQueue D_80128370;
extern OSMesg D_80128388[];
extern OSThread D_80128390;
extern OSMesgQueue D_80128540;
void D_002A2650(void *arg);
void func_002A24B4(s32 pri) {
    if (D_800BC300 == 0) {
        D_800BC300 = 1;
        func_0029B030(&D_801280F0, D_801280E4, 1);
        func_0029B030(&D_80128370, D_801280E0, 1);
        func_0029B030(&D_80128540, D_80128388, 1);
        func_0029B030(&D_801270C0, D_801270B4, 1);
        func_0029B950(0, &D_801270C0, (OSMesg)1);
        func_0029B060(&D_80128390, 0x4653, D_002A2650, 0, D_801280DC + 4, pri);
        func_0029BB10(&D_80128390);
    }
}
