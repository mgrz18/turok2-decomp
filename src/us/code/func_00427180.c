/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_2.c, func_80445F80_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00429DFC(OSPfs *pfs);
extern s32 func_004292E0(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 *file_no);
extern s32 osPfsFreeBlocks(OSPfs *pfs, s32 *bytes_not_used);
extern s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 func_00427524(OSPfs *pfs, __OSInode *inode, int file_size_in_pages, int *first_page, u8 bank,
                         int *decleared, int *last_page);
extern s32 func_00429DFC(OSPfs *pfs);
extern s32 func_004292E0(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 *file_no);
extern s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
s32 func_00427180(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name,
                  int file_size_in_bytes, s32 *file_no)
{
    int start_page;
    int decleared;
    int last_page;
    int old_last_page = 0;
    int j;
    s32 ret = 0;
    int file_size_in_pages;
    __OSInode inode;
    __OSInode backup_inode;
    __OSDir dir;
    u8 bank;
    u8 old_bank = 0;
    int firsttime = 0;
    s32 bytes;
    __OSInodeUnit fpage;
    if (company_code == 0 || game_code == 0) {
        return 5;
    }
    file_size_in_pages = (file_size_in_bytes + 32 * 8 - 1) / (32 * 8);
    if ((pfs->status & 1) == 0) {
        return 5;
    }
    if (func_00429DFC(pfs) == 2)
        return 2;
    if (((ret = func_004292E0(pfs, company_code, game_code, game_name, ext_name, file_no)) != 0) &&
        ret != 5) {
        return ret;
    }
    if (*file_no != -1) {
        return 9;
    }
    ret = osPfsFreeBlocks(pfs, &bytes);
    if (file_size_in_bytes > bytes) {
        return 7;
    }
    if (file_size_in_pages != 0) {
        if (((ret = func_004292E0(pfs, 0, 0, 0, 0, file_no)) != 0) && ret != 5) {
            return ret;
        }
        if (*file_no == -1) {
            return 8;
        }
        for (bank = 0; bank < pfs->banks; bank++) {
            ret = func_00429AA4(pfs, &inode, 0, bank);
            if (ret != 0)
                return ret;
            ret = func_00427524(pfs, &inode, file_size_in_pages, &start_page, bank, &decleared, &last_page);
            if (ret != 0)
                return ret;
            if (start_page != -1) {
                if (firsttime == 0) {
                    fpage.inode_t.page = start_page;
                    fpage.inode_t.bank = bank;
                } else {
                    backup_inode.inode_page[old_last_page].inode_t.bank = bank;
                    backup_inode.inode_page[old_last_page].inode_t.page = start_page;
                    ret = func_00429AA4(pfs, &backup_inode, 1, old_bank);
                    if (ret != 0)
                        return ret;
                }
                for (j = 0; j < 128; j++) {
                    backup_inode.inode_page[j].ipage = inode.inode_page[j].ipage;
                }
                old_last_page = last_page;
                old_bank = bank;
                firsttime++;
                if (file_size_in_pages > decleared) {
                    file_size_in_pages = file_size_in_pages - decleared;
                } else {
                    file_size_in_pages = 0;
                    break;
                }
            }
        }
        if (file_size_in_pages > 0 || start_page == -1) {
            return 3;
        }
        backup_inode.inode_page[old_last_page].inode_t.bank = bank;
        backup_inode.inode_page[old_last_page].inode_t.page = start_page;
        ret = func_00429AA4(pfs, &backup_inode, 1, old_bank);
        if (ret != 0)
            return ret;
        dir.start_page = fpage;
        dir.company_code = company_code;
        dir.game_code = game_code;
        dir.data_sum = 0;
        for (j = 0; j < 16; j++)
            dir.game_name[j] = *game_name++;
        for (j = 0; j < 4; j++)
            dir.ext_name[j] = *ext_name++;
        ret = func_00299A40(pfs->queue, pfs->channel, pfs->dir_table + *file_no, (u8 *)&dir, 0);
        if (ret != 0)
            return ret;
        return ret;
    } else {
        return 5;
    }
}
s32 func_00427524(OSPfs *pfs, __OSInode *inode, int file_size_in_pages, int *first_page, u8 bank,
                  int *decleared, int *last_page);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
