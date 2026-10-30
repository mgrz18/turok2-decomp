/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/PFS/80446BD0_4.CPP, func_804467AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
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
extern s32 __osPfsReleasePages(OSPfs *pfs, __OSInode *inode, u8 start_page, u16 *sum, u8 bank,
                         __OSInodeUnit *last_page, int flag);
extern s32 func_00427A6C(OSPfs *pfs, u8 page_no, u16 *sum, u8 bank);
s32 __osPfsReleasePages(OSPfs *pfs, __OSInode *inode, u8 start_page, u16 *sum, u8 bank,
                  __OSInodeUnit *last_page, int flag)
{
    __OSInodeUnit next_page;
    __OSInodeUnit old_page;
    s32 ret;
    int offset;
    ret = 0;
    next_page = inode->inode_page[start_page];
    if (next_page.ipage != 1) {
        offset = (next_page.inode_t.bank > 0) ? 1 : pfs->inode_start_page;
    } else {
        offset = (bank > 0) ? 1 : pfs->inode_start_page;
    }
    if (next_page.inode_t.page < offset && next_page.ipage != 1) {
        return 3;
    }
    *last_page = next_page;
    if (flag == 1) {
        inode->inode_page[start_page].ipage = 3;
    }
    ret = func_00427A6C(pfs, start_page, sum, bank);
    if (ret != 0)
        return ret;
    if (next_page.ipage == 1) {
        return 0;
    }
    while (next_page.ipage >= pfs->inode_start_page) {
        old_page = next_page;
        next_page = inode->inode_page[next_page.inode_t.page];
        inode->inode_page[old_page.inode_t.page].ipage = 3;
        ret = func_00427A6C(pfs, old_page.inode_t.page, sum, bank);
        if (ret != 0)
            return ret;
        if (next_page.inode_t.bank != bank) {
            break;
        }
    }
    if (next_page.ipage >= pfs->inode_start_page && next_page.inode_t.bank == bank) {
        inode->inode_page[next_page.inode_t.page].ipage = 3;
    }
    *last_page = next_page;
    return 0;
}
s32 func_00427A6C(OSPfs *pfs, u8 page_no, u16 *sum, u8 bank);
