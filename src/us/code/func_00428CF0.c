/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/osPfsReadWriteFile.c, osPfsReadWriteFile.
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
s32 func_00299680(OSMesgQueue *mq, int channel, u16 address, u8 *buffer);
s32 func_00299A40(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
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
typedef struct {
               u32 game_code;
               u16 company_code;
               __OSInodeUnit start_page;
               u8 status;
               s8 reserved;
               u16 data_sum;
               u8 ext_name[4];
               u8 game_name[16];
} __OSDir;
s32 func_00429DFC(OSPfs *pfs);
s32 func_00429EC4(OSPfs *pfs);
s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
static __inline__ s32 next_page(OSPfs *pfs, u8 *cbank, __OSInode *inode, __OSInodeUnit *curr) {
    s32 ret;
    u8 b;
    if (curr->inode_t.bank != *cbank) {
        *cbank = curr->inode_t.bank;
        ret = func_00429AA4(pfs, inode, 0, *cbank);
        if (ret != 0) {
            return ret;
        }
    }
    *curr = inode->inode_page[curr->inode_t.page];
    if (curr->ushort < pfs->inode_start_page || curr->inode_t.bank >= pfs->banks || curr->inode_t.page == 0 || (s8)curr->inode_t.page < 0) {
        if (curr->ushort == 1) {
            return 5;
        }
        return 3;
    }
    return 0;
}
s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, int offset, int size_in_bytes, u8 *data_buffer) {
    s32 ret;
    __OSDir dir;
    __OSInode inode;
    u8 cbank;
    __OSInodeUnit curr_page;
    s32 offset_in_blocks;
    s32 size_in_blocks;
    u8 *buf;
    s32 addr;
    if (file_no >= pfs->dir_size || file_no < 0) {
        return 5;
    }
    if (size_in_bytes <= 0 || (size_in_bytes & 0x1F) != 0) {
        return 5;
    }
    if (offset < 0 || (offset & 0x1F) != 0) {
        return 5;
    }
    if ((pfs->status & 0x1) == 0) {
        return 5;
    }
    if (func_00429DFC(pfs) == 2) {
        return 2;
    }
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = func_00429EC4(pfs);
        if (ret != 0) {
            return ret;
        }
    }
    ret = func_00299680(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir);
    if (ret != 0) {
        return ret;
    }
    if (dir.company_code == 0 || dir.game_code == 0) {
        return 5;
    }
    if (dir.start_page.ushort < pfs->inode_start_page || dir.start_page.inode_t.bank >= pfs->banks || dir.start_page.inode_t.page == 0 || (s8)dir.start_page.inode_t.page < 0) {
        if (dir.start_page.ushort == 1) {
            return 5;
        }
        return 3;
    }
    if (flag == 0 && (dir.status & 2) == 0) {
        return 6;
    }
    cbank = 0xFF;
    offset_in_blocks = offset / 32;
    curr_page.ushort = dir.start_page.ushort;
    while (offset_in_blocks >= 8) {
        ret = next_page(pfs, &cbank, &inode, &curr_page);
        if (ret != 0) {
            return ret;
        }
        offset_in_blocks -= 8;
    }
    size_in_blocks = size_in_bytes / 32;
    buf = data_buffer;
    while (size_in_blocks > 0) {
        if (offset_in_blocks == 8) {
            ret = next_page(pfs, &cbank, &inode, &curr_page);
            if (ret != 0) {
                return ret;
            }
            offset_in_blocks = 0;
        }
        if (pfs->activebank != curr_page.inode_t.bank) {
            pfs->activebank = curr_page.inode_t.bank;
            ret = func_00429EC4(pfs);
            if (ret != 0) {
                return ret;
            }
        }
        addr = curr_page.inode_t.page * 8 + offset_in_blocks;
        if (flag == 0) {
            ret = func_00299680(pfs->queue, pfs->channel, (u16)addr, buf);
        } else {
            ret = func_00299A40(pfs->queue, pfs->channel, (u16)addr, buf, 0);
        }
        if (ret != 0) {
            return ret;
        }
        buf += 32;
        size_in_blocks--;
        offset_in_blocks++;
    }
    if (flag == 1 && (dir.status & 2) == 0) {
        dir.status |= 2;
        pfs->activebank = 0;
        ret = func_00429EC4(pfs);
        if (ret != 0) {
            return ret;
        }
        ret = func_00299A40(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir, 0);
        if (ret != 0) {
            return ret;
        }
    }
    return 0;
}
