/* Second-module copy of osPfsFreeBlocks (first module; this one at link address 0x00427D80), calling this module's own functions at their link addresses. */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/osPfsFreeBlocks_2.c, osPfsFreeBlocks_2.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
s32 func_00429D1C(OSPfs *pfs);
s32 func_004299C4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 osPfsFreeBlocks_2(OSPfs *pfs, s32 *leo_bytes) {
    __OSInode inode;
    s32 j;
    u8 bank;
    s32 offset;
    s32 free_pages = 0;
    s32 ret;
    if ((pfs->status & 0x1) == 0) {
        return 5;
    }
    if (func_00429D1C(pfs) == 2) {
        return 2;
    }
    for (bank = 0; bank < pfs->banks; bank++) {
        ret = func_004299C4(pfs, &inode, 0, bank);
        if (ret != 0) {
            return ret;
        }
        offset = (bank == 0) ? pfs->inode_start_page : 1;
        for (j = offset; j < 128; j++) {
            if (inode.inode_page[j].ipage == 3) {
                free_pages++;
            }
        }
    }
    *leo_bytes = free_pages * 256;
    return 0;
}
