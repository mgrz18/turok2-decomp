/* From decompals/ultralib (libultra 2.0I): src/io/pfschecker.c, corrupted.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef long s32;
typedef float f32;
typedef double f64;
typedef s32 OSPri;
typedef s32 OSId;
typedef union {
    struct {
        f32 f_odd;
        f32 f_even;
    } f;
    f64 d;
} __OSfp;
typedef struct {
    u64 at, v0, v1, a0, a1, a2, a3;
    u64 t0, t1, t2, t3, t4, t5, t6, t7;
    u64 s0, s1, s2, s3, s4, s5, s6, s7;
    u64 t8, t9;
    u64 gp, sp, s8, ra;
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
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct {
 int status;
 OSMesgQueue *queue;
 int channel;
 u8 id[32];
 u8 label[32];
 int version;
 int dir_size;
 int inode_table;
 int minode_table;
 int dir_table;
 int inode_start_page;
 u8 banks;
 u8 activebank;
} OSPfs;
typedef union {
              struct
    {
                  u8 bank;
                  u8 page;
    } inode_t;
              u16 ipage;
} __OSInodeUnit;
typedef struct
{
              __OSInodeUnit inode_page[128];
} __OSInode;
typedef struct
{
              __OSInode inode;
                u8 bank;
                u8 map[(8 * 32)];
} __OSInodeCache;
s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 corrupted(OSPfs* pfs, __OSInodeUnit fpage, __OSInodeCache* cache);
s32 corrupted(OSPfs* pfs, __OSInodeUnit fpage, __OSInodeCache* cache) {
    int j;
    int n;
    int hit;
    u8 bank;
    int offset;
    s32 ret;
    hit = 0;
    ret = 0;
    n = (fpage.inode_t.page / (128/32)) + (fpage.inode_t.bank % 8) * 32;
    for (bank = 0; bank < pfs->banks; bank++) {
        offset = bank > 0 ? 1 : pfs->inode_start_page;
        if (bank == fpage.inode_t.bank || cache->map[n] & (1 << (bank % 8))) {
            if (bank != cache->bank) {
                ret = func_00429AA4(pfs, &cache->inode, 0, bank);
                if (ret != 0 && ret != 3) {
                    return ret;
                }
                cache->bank = bank;
            }
            for (j = offset; hit < 2 && (j < ((s32)(sizeof(cache->inode.inode_page) / sizeof(cache->inode.inode_page[0])))); j++) {
                if (cache->inode.inode_page[j].ipage == fpage.ipage) {
                    hit++;
                }
            }
            if (hit >= 2) {
                return 2;
            }
        }
    }
    return hit;
}
