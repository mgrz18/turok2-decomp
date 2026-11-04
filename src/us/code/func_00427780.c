/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/osPfsDeleteFile.c, osPfsDeleteFile.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
s32 func_00299680(OSMesgQueue *mq, int channel, u16 address, u8 *buffer);
s32 func_00299A40(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
s32 func_00429DFC(OSPfs *pfs);
s32 func_00429EC4(OSPfs *pfs);
s32 func_004292E0(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 *file_no);
s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 func_004279AC(OSPfs *pfs, __OSInode *inode, u8 initial_page, u16 *sum, u8 bank, __OSInodeUnit *final_page, s32 flag);
s32 osPfsDeleteFile(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name) {
    s32 file_no;
    s32 ret;
    __OSInode inode;
    __OSDir dir;
    __OSInodeUnit last_page;
    u8 startpage;
    u8 bank;
    u16 sum;
    s32 j;
    sum = 0;
    if (company_code == 0 || game_code == 0) {
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
    ret = func_004292E0(pfs, company_code, game_code, game_name, ext_name, &file_no);
    if (ret != 0) {
        return ret;
    }
    if (file_no == -1) {
        return 5;
    }
    ret = func_00299680(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir);
    if (ret != 0) {
        return ret;
    }
    startpage = dir.start_page.inode_t.page;
    bank = dir.start_page.inode_t.bank;
    while (bank < pfs->banks) {
        ret = func_00429AA4(pfs, &inode, 0, bank);
        if (ret != 0) {
            return ret;
        }
        ret = func_004279AC(pfs, &inode, startpage, &sum, bank, &last_page, 1);
        if (ret != 0) {
            return ret;
        }
        ret = func_00429AA4(pfs, &inode, 1, bank);
        if (ret != 0) {
            return ret;
        }
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
    for (j = 0; j < 16; j++) {
        dir.game_name[j] = 0;
    }
    for (j = 0; j < 4; j++) {
        dir.ext_name[j] = 0;
    }
    dir.status = 0;
    return func_00299A40(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir, 0);
}
