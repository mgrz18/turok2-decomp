/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/PFS/80447BB0_19.CPP, func_804488A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
typedef struct OSPfs OSPfs;
struct OSPfs {
    int status;
    struct OSMesgQueue *queue;
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
};
struct OSPfs;
struct __OSInode;
typedef struct __OSInode __OSInode;
union __OSInodeUnit;
union __OSInodeUnit {
    struct {
        u8 bank;
        u8 page;
    } inode_t;
    u16 ipage;
};
union __OSInodeUnit;
typedef union __OSInodeUnit __OSInodeUnit;
struct __OSInode;
struct __OSInode {
    __OSInodeUnit inode_page[128];
};
struct OSMesgQueue;
struct OSMesgQueue;
extern s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 func_00429DE4(OSPfs *pfs);
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
static __inline__ u16 __osSumcalc(u8 *ptr, int length) {
    int i;
    u32 sum = 0;
    u8 *tmp = ptr;
    for (i = 0; i < length; i++) {
        sum += *tmp++;
        sum = sum & 0xFFFF;
    }
    return sum;
}
s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank)
{
    u8 sum;
    int j;
    s32 ret;
    int offset;
    u8 *addr;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = func_00429DE4(pfs);
        if (ret != 0)
            return ret;
    }
    offset = (bank > 0) ? 1 : pfs->inode_start_page;
    if (flag == 1) {
        inode->inode_page[0].inode_t.page = __osSumcalc((u8 *)&inode->inode_page[offset], (128 - offset) * 2);
    }
    for (j = 0; j < 8; j++) {
        addr = ((u8 *)inode->inode_page + j * 32);
        if (flag == 1) {
            ret = func_00299A40(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr, 0);
            ret = func_00299A40(pfs->queue, pfs->channel, pfs->minode_table + bank * 8 + j, addr, 0);
        } else {
            ret = func_00299680(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr);
        }
        if (ret != 0) {
            return ret;
        }
    }
    if (flag == 0) {
        sum = __osSumcalc((u8 *)&inode->inode_page[offset], (128 - offset) * 2);
        if (sum != inode->inode_page[0].inode_t.page) {
            for (j = 0; j < 8; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = func_00299680(pfs->queue, pfs->channel, pfs->minode_table + bank * 8 + j, addr);
            }
            if (sum != inode->inode_page[0].inode_t.page) {
                return 3;
            }
            for (j = 0; j < 8; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = func_00299A40(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr, 0);
            }
        } else {
            for (j = 0; j < 8; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = func_00299A40(pfs->queue, pfs->channel, pfs->minode_table + bank * 8 + j, addr, 0);
            }
        }
    }
    return 0;
}
