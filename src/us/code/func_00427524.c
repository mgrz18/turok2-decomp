/* WRONG CUT: REAL func_00427524 size 0x1CC (splat cut it into func_00427524 + func_00427610 + func_004276A0). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SDK/PFS/80446BD0_4.CPP, func_80446324_de, with Turok 2's layouts. */
/* __osPfsDeclearPage (libultra pfsallocatefile.c). */
#include "rw/abu_types.h"

extern s32 func_004276F0(OSPfs *pfs, int page_no, u8 *data, u8 bank);

s32 func_00427524(OSPfs *pfs, __OSInode *inode, int file_size_in_pages, int *first_page, u8 bank,
                  int *decleared, int *last_page)
{
    int j;
    int spage;
    int old_page;
    u8 tmp_data[32];
    int i;
    s32 ret = 0;
    int offset = bank > 0 ? 1 : pfs->inode_start_page;

    for (j = offset; j < 128; j++) {
        if (inode->inode_page[j].ipage == 3) {
            break;
        }
    }

    if (j == 128) {
        *first_page = -1;
        return ret;
    }

    for (i = 0; i < 32; i++) {
        tmp_data[i] = 0;
    }

    spage = j;
    *decleared = 1;
    old_page = j;
    j++;

    while (file_size_in_pages > *decleared && j < 128) {
        if (inode->inode_page[j].ipage == 3) {
            inode->inode_page[old_page].inode_t.bank = bank;
            inode->inode_page[old_page].inode_t.page = j;
            ret = func_004276F0(pfs, old_page, tmp_data, bank);
            if (ret != 0)
                return ret;
            old_page = j;
            (*decleared)++;
        }
        j++;
    }

    *first_page = spage;

    if (j == 128 && file_size_in_pages > *decleared) {
        *last_page = old_page;
        return ret;
    } else {
        inode->inode_page[old_page].ipage = 1;
        ret = func_004276F0(pfs, old_page, tmp_data, bank);
        *last_page = 0;
        return ret;
    }
}
