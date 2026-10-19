/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_4.c, func_80446A20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00429DFC(OSPfs *pfs);
extern s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 func_00429EC4(OSPfs *pfs);
extern s32 func_00429EC4(OSPfs *pfs);
extern s32 func_00429DFC(OSPfs *pfs);
extern s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 func_00429EC4(OSPfs *pfs);
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
s32 func_00427C20(OSPfs *pfs, s32 file_no, NoteState *state)
{
    s32 ret;
    int pages;
    __OSInode inode;
    __OSDir dir;
    __OSInodeUnit next_page;
    int j;
    u8 bank;
    u8 start_page;
    if (file_no >= pfs->dir_size || file_no < 0) {
        return 5;
    }
    if ((pfs->status & 1) == 0)
        return 5;
    if (func_00429DFC(pfs) == 2)
        return 2;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = func_00429EC4(pfs);
        if (ret != 0)
            return ret;
    }
    ret = func_00299680(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8 *)&dir);
    if (ret != 0)
        return ret;
    if (dir.company_code == 0 || dir.game_code == 0) {
        return 5;
    }
    if (dir.start_page.ipage < pfs->inode_start_page) {
        return 3;
    }
    pages = 0;
    start_page = dir.start_page.inode_t.page;
    bank = dir.start_page.inode_t.bank;
    while (bank < pfs->banks) {
        ret = func_00429AA4(pfs, &inode, 0, bank);
        if (ret != 0)
            return ret;
        next_page = inode.inode_page[start_page];
        pages++;
        while (next_page.ipage >= pfs->inode_start_page) {
            pages++;
            next_page = inode.inode_page[next_page.inode_t.page];
            if (next_page.inode_t.bank != bank) {
                bank = next_page.inode_t.bank;
                start_page = next_page.inode_t.page;
                break;
            }
        }
        if (next_page.ipage == 1) {
            break;
        }
    }
    if (next_page.ipage != 1) {
        return 3;
    }
    state->file_size = pages * (8 * 32);
    state->company_code = dir.company_code;
    state->game_code = dir.game_code;
    for (j = 0; j < 16; j++) {
        state->game_name[j] = dir.game_name[j];
    }
    for (j = 0; j < 4; j++) {
        state->ext_name[j] = dir.ext_name[j];
    }
    return 0;
}
