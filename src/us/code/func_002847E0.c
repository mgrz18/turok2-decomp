/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00263760.c, func_00263760.
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
typedef struct {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;
typedef struct {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;
typedef struct {
               s32 status;
               OSMesgQueue *queue;
               s32 channel;
               u8 id[32];
               u8 label[32];
               s32 version;
               s32 dir_size;
               s32 inode_table;
               s32 minode_table;
               s32 dir_table;
               s32 inode_start_page;
               u8 banks;
               u8 activebank;
} OSPfs;
void func_002987B4(OSContPad *pad);
s32 func_00298900(OSMesgQueue *mq, OSContStatus *data);
void func_002986F8(OSContStatus *data);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
typedef struct B14Ent { u8 a, b, c, d; } B14Ent;
typedef struct B14Ctl {
                s32 f0;
                s8 port;
                u8 pad5[3];
                s32 f8;
                s32 fC;
                s32 f10;
                s32 f14;
                s32 f18;
                s32 f1C;
                s32 f20;
                s32 f24;
                s32 f28;
                B14Ent ent[32];
                s32 fAC;
                s32 fB0;
                s32 fB4;
                s32 fB8;
                s32 fBC;
                s32 fC0;
                u8 fC4;
                u8 fC5;
                u8 fC6;
                u8 fC7;
                s32 fC8;
                s32 fCC;
                union { s32 i; f32 f; } fD0;
                union { s32 i; f32 f; } fD4;
                OSPfs pfs;
                u8 blkA[0x2C];
                u8 blkB[0xB4];
                s32 f220;
} B14Ctl;
typedef struct M04Pad {
    u16 button;
    s8 x;
    s8 y;
    u8 err;
    u8 pad;
} M04Pad;
extern f32 D_800A9CE0;
extern B14Ctl D_800F5C78;
extern B14Ctl D_800F5EC0[];
extern M04Pad D_800F5C60[];
extern u8 D_800F6750;
extern OSMesgQueue D_800F6798;
extern OSMesg D_800F6770;
extern u8 D_800F6778[];
void func_00284DAC(B14Ctl *c);
void func_00284EB8(B14Ctl *c, s8 x, s8 y, u16 b);
void func_002847E0(void) {
    s32 i;
    switch (D_800F6750) {
    case 0:
        osRecvMesg(&D_800F6798, &D_800F6770, 1);
        func_002987B4((OSContPad *)D_800F5C60);
        D_800F5C78.port = -1;
        D_800F5C78.f0 = 0;
        D_800F5C78.fC4 = 0;
        D_800F5C78.fC5 = 0;
        D_800F5C78.fC6 = 0;
        D_800F5C78.fC7 = 0;
        D_800F5C78.fB0 = 0;
        D_800F5C78.fAC = 0;
        D_800F5C78.fB4 = 0;
        D_800F5C78.fB8 = 0;
        D_800F5C78.fBC = 0;
        D_800F5C78.fC0 = 0;
        D_800F5C78.f18 = 0;
        D_800F5C78.f14 = 0;
        D_800F5C78.f1C = 0;
        D_800F5C78.f20 = 0;
        D_800F5C78.f24 = 0;
        D_800F5C78.f28 = 0;
        for (i = 0; i < 4; i++) {
            func_00284EB8(&D_800F5EC0[i], D_800F5C60[i].x, D_800F5C60[i].y, D_800F5C60[i].button);
            D_800F5C78.f0 |= D_800F5EC0[i].f0;
            D_800F5C78.fC4 += D_800F5EC0[i].fC4;
            D_800F5C78.fC5 += D_800F5EC0[i].fC5;
            D_800F5C78.fC6 += D_800F5EC0[i].fC6;
            D_800F5C78.fC7 += D_800F5EC0[i].fC7;
            D_800F5C78.fB0 |= D_800F5EC0[i].fB0;
            D_800F5C78.fB4 |= D_800F5EC0[i].fB4;
            D_800F5C78.fB8 |= D_800F5EC0[i].fB8;
            D_800F5C78.fBC |= D_800F5EC0[i].fBC;
            D_800F5C78.fC0 |= D_800F5EC0[i].fC0;
            D_800F5C78.f18 |= D_800F5EC0[i].f18;
            D_800F5C78.f14 |= D_800F5EC0[i].f14;
            D_800F5C78.f1C |= D_800F5EC0[i].f1C;
            D_800F5C78.f20 |= D_800F5EC0[i].f20;
            D_800F5C78.f24 |= D_800F5EC0[i].f24;
            D_800F5C78.f28 |= D_800F5EC0[i].f28;
        }
        D_800F5C78.fC4 = (s8)((f32)(s8)D_800F5C78.fC4 * D_800A9CE0);
        D_800F5C78.fC5 = (s8)((f32)(s8)D_800F5C78.fC5 * D_800A9CE0);
        D_800F5C78.fC6 = (s8)((f32)(s8)D_800F5C78.fC6 * D_800A9CE0);
        D_800F5C78.fC7 = (s8)((f32)(s8)D_800F5C78.fC7 * D_800A9CE0);
        break;
    case 1:
        osRecvMesg(&D_800F6798, &D_800F6770, 1);
        func_002986F8((OSContStatus *)D_800F6778);
        for (i = 0; i < 4; i++) {
            func_00284DAC(&D_800F5EC0[i]);
        }
        break;
    case 3:
        func_00298900(&D_800F6798, (OSContStatus *)D_800F6778);
    case 2:
        break;
    }
}
