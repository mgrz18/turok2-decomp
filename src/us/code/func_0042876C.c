/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446F60_de.c, func_8044756C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 func_0042876C(OSPfs *pfs, __OSInodeCache *cache);
extern s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 func_0042876C(OSPfs *pfs, __OSInodeCache *cache)
{
    int i;
    int n;
    int offset;
    u8 bank;
    __OSInodeUnit tpage;
    __OSInode tmp_inode;
    s32 ret;
    for (i = 0; i < 256; i++) {
        cache->map[i] = 0;
    }
    cache->bank = -1;
    for (bank = 0; bank < pfs->banks; bank++) {
        offset = bank > 0 ? 1 : pfs->inode_start_page;
        ret = func_00429AA4(pfs, &tmp_inode, 0, bank);
        if (ret != 0 && ret != 3) {
            return ret;
        }
        for (i = offset; i < 128; i++) {
            tpage = tmp_inode.inode_page[i];
            if (tpage.ipage >= pfs->inode_start_page && tpage.inode_t.bank != bank) {
                n = ((tpage.inode_t.page) / 4) + ((tpage.inode_t.bank % 8) * 32);
                cache->map[n] |= 1 << (bank % 8);
            }
        }
    }
    return 0;
}
