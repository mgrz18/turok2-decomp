/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/osPfsFreeBlocks.c, osPfsFreeBlocks.
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
typedef union {
    struct {
        u8 bank;
        u8 page;
    } inode_t;
    u16 ushort;
} __OSInodeUnit;
typedef struct {
               __OSInodeUnit inode_page[128];
} __OSInode;
s32 func_00429DFC(OSPfs *pfs);
s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 osPfsFreeBlocks(OSPfs *pfs, s32 *leo_bytes) {
    __OSInode inode;
    s32 j;
    u8 bank;
    s32 offset;
    s32 free_pages = 0;
    s32 ret;
    if ((pfs->status & 0x1) == 0) {
        return 5;
    }
    if (func_00429DFC(pfs) == 2) {
        return 2;
    }
    for (bank = 0; bank < pfs->banks; bank++) {
        ret = func_00429AA4(pfs, &inode, 0, bank);
        if (ret != 0) {
            return ret;
        }
        offset = (bank == 0) ? pfs->inode_start_page : 1;
        for (j = offset; j < 128; j++) {
            if (inode.inode_page[j].ushort == 3) {
                free_pages++;
            }
        }
    }
    *leo_bytes = free_pages * 256;
    return 0;
}
