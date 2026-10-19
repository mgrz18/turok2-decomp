/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00404018.c, func_00404018.
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
typedef struct O01Pad4 {
            u8 pad0[3];
            u8 unk3;
} O01Pad4;
extern O01Pad4 D_800F6778[];
extern OSMesgQueue D_800F6798;
extern OSPfs D_80130C80[];
extern s32 D_80130C70[];
extern s32 D_80130C60[];
s32 func_00428160(OSMesgQueue *queue, OSPfs *pfs, int channel);
s32 func_00425760(s32 pad);
s32 func_004258E8(s32 pad) {
    s32 i;
    s32 r;
    s32 v;
    s32 code;
    s32 ok;
    s32 ret;
    if (D_800F6778[pad].unk3 != 0) {
        D_80130C70[pad] = -2;
        ret = 0;
    } else {
        i = 0;
        do {
            r = func_00428160(&D_800F6798, &D_80130C80[pad], pad);
            switch (r) {
            case 0:
            case 2:
                D_80130C70[pad] = -5;
                break;
            case 1:
                D_80130C70[pad] = -2;
                break;
            case 4:
            case 11:
                code = -3;
                goto st;
            case 10:
                code = -4;
            st:
                D_80130C70[pad] = code;
                break;
            }
            if (D_80130C70[pad] == -5) {
                goto got5;
            }
            if (D_80130C70[pad] == -2) {
                goto fin;
            }
            i++;
        } while (i < 4);
        goto fin2;
    got5:
        D_80130C60[pad] = 3;
        if (func_00425760(pad) != 0) {
            D_80130C70[pad] = -4;
            goto fin2;
        }
        D_80130C70[pad] = 0;
    fin:
    fin2:
        v = D_80130C70[pad];
        ok = 0;
        if (v == 0 || v == -4) {
            ok = 1;
        }
        ret = ok;
    }
    return ret;
}
