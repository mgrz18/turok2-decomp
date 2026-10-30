/* WRONG CUT: REAL func_004279AC size 0x1A0 (splat cut it into func_004279AC + func_00427A6C + func_00427B40). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SDK/PFS/80446BD0_4.CPP, func_004279AC, with Turok 2's layouts. */
/* __osPfsReleasePages (libultra pfsdeletefile.c). */
#include "rw/abu_types.h"

extern s32 func_00427B4C(OSPfs *pfs, u8 page_no, u16 *sum, u8 bank);

s32 func_004279AC(OSPfs *pfs, __OSInode *inode, u8 start_page, u16 *sum, u8 bank,
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

    ret = func_00427B4C(pfs, start_page, sum, bank);
    if (ret != 0)
        return ret;

    if (next_page.ipage == 1) {
        return 0;
    }

    while (next_page.ipage >= pfs->inode_start_page) {
        old_page = next_page;
        next_page = inode->inode_page[next_page.inode_t.page];
        inode->inode_page[old_page.inode_t.page].ipage = 3;

        ret = func_00427B4C(pfs, old_page.inode_t.page, sum, bank);
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
