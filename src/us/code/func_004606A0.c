/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/PFS/80446BD0_4.CPP, func_80446580_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
extern s32 func_004278CC(OSPfs *pfs, __OSInode *inode, u8 start_page, u16 *sum, u8 bank,
                         __OSInodeUnit *last_page, int flag);
extern s32 func_00429200(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 *file_no);
extern s32 func_004299C4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 func_00429D1C(OSPfs *pfs);
extern s32 func_00429DE4(OSPfs *pfs);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
s32 osPfsDeleteFile_2(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name)
{
    s32 file_no;
    int k;
    s32 ret;
    __OSInode inode;
    __OSDir dir;
    u16 sum = 0;
    __OSInodeUnit last_page;
    u8 startpage;
    u8 bank;
    if (company_code == 0 || game_code == 0) {
        return 5;
    }
    if ((pfs->status & 1) == 0)
        return 5;
    if (func_00429D1C(pfs) == 2)
        return 2;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = func_00429DE4(pfs);
        if (ret != 0)
            return ret;
    }
    ret = func_00429200(pfs, company_code, game_code, game_name, ext_name, &file_no);
    if (ret != 0)
        return ret;
    if (file_no == -1) {
        return 5;
    }
    ret = func_00299680(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8 *)&dir);
    if (ret != 0)
        return ret;
    startpage = dir.start_page.inode_t.page;
    for (bank = dir.start_page.inode_t.bank; bank < pfs->banks;) {
        ret = func_004299C4(pfs, &inode, 0, bank);
        if (ret != 0)
            return ret;
        ret = func_004278CC(pfs, &inode, startpage, &sum, bank, &last_page, 1);
        if (ret != 0)
            return ret;
        ret = func_004299C4(pfs, &inode, 1, bank);
        if (ret != 0)
            return ret;
        if (last_page.ipage == 1) {
            break;
        }
        bank = last_page.inode_t.bank;
        startpage = last_page.inode_t.page;
    }
    if (bank >= pfs->banks) {
        return 3;
    }
    dir.game_code = 0;
    dir.company_code = 0;
    dir.start_page.ipage = 0;
    dir.data_sum = 0;
    for (k = 0; k < 16; k++) {
        dir.game_name[k] = 0;
    }
    for (k = 0; k < 4; k++) {
        dir.ext_name[k] = 0;
    }
    dir.status = 0;
    ret = func_00299A40(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8 *)&dir, 0);
    return ret;
}
s32 func_004278CC(OSPfs *pfs, __OSInode *inode, u8 start_page, u16 *sum, u8 bank,
                  __OSInodeUnit *last_page, int flag);
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
