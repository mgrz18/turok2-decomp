/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201DB0.c, func_00201DB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

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
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
typedef struct B01IoReq {
               s32 w0;
               s32 w4;
               s32 w8;
               union { s32 i; struct { u16 hi; u16 cmd; } h; } wC;
               s32 w10;
               s32 w14;
               s32 w18;
               OSMesgQueue *reply;
} B01IoReq;
extern s32 D_800BC304;
extern OSMesgQueue D_80128370;
extern OSMesgQueue D_80128540;
s32 func_002A12F0(void);
void func_002A1324(s32 mask);
s32 func_002A259C(u32 a0, u32 a1, u32 a2, u32 a3);
s32 func_002A2B08(s32 arg) {
    B01IoReq req;
    s32 reply;
    s32 mask;
    if (D_800BC304 == 2) {
        req.w4 = 0;
        req.wC.i = 0x80000404;
        req.w10 = arg;
        req.w14 = 0;
        req.w18 = 0;
        req.reply = &D_80128540;
        osSendMesg(&D_80128370, &req, 1);
        osRecvMesg(&D_80128540, (OSMesg *)&reply, 1);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
    } else {
        mask = func_002A12F0();
        reply = func_002A259C(0x404, arg, 0, 0);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
        func_002A1324(mask);
    }
    return reply;
}
