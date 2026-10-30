/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00404858.c, func_00404858.
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
typedef struct O01PadEnt {
               s32 unk0;
               s32 unk4;
               u16 unk8;
               u8 unkA[4];
               u8 unkE[0x12];
} O01PadEnt;
typedef struct O01PadTab {
                s32 unk0;
                O01PadEnt ent[16];
} O01PadTab;
extern s32 D_80130C60[];
extern s32 D_80130C70[];
extern OSPfs D_80130C80[];
extern O01PadTab *D_800C2224;
extern u8 D_800F6750;
s32 func_00285878(s32 flag);
void func_002847E0();
void func_002858A4(void);
s32 func_00425808(s32 ch);
s32 func_004276A0(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);
s32 func_0045EF84(s32 pad, s32 idx) {
    s32 r;
    O01PadEnt *e;
    if (D_80130C60[pad] != 3) {
        return -2;
    }
    func_00285878(1);
    func_002847E0();
    r = D_80130C70[pad];
    D_800F6750 = 2;
    if (r == 0) {
        r = func_004276A0(&D_80130C80[pad], D_800C2224[pad].ent[idx].unk8, D_800C2224[pad].ent[idx].unk4, D_800C2224[pad].ent[idx].unkE, D_800C2224[pad].ent[idx].unkA);
        if (r != 0) {
            r = -1;
        }
        func_00425808(pad);
    }
    func_002858A4();
    return r;
}
