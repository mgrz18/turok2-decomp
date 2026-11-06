/* Second-module copy of osPfsReadWriteFile (first module; this one at link address 0x00428C10), calling this module's own functions at their link addresses. */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/osPfsReadWriteFile_2.c, osPfsReadWriteFile_2.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
s32 __osContRamRead(OSMesgQueue *mq, int channel, u16 address, u8 *buffer);
s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
s32 __osCheckId_2(OSPfs *pfs);
s32 __osPfsSelectBank_2(OSPfs *pfs);
s32 __osPfsRWInode_2(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
static __inline__ s32 next_page(OSPfs *pfs, u8 *cbank, __OSInode *inode, __OSInodeUnit *curr) {
    s32 ret;
    u8 b;
    if (curr->inode_t.bank != *cbank) {
        *cbank = curr->inode_t.bank;
        ret = __osPfsRWInode_2(pfs, inode, 0, *cbank);
        if (ret != 0) {
            return ret;
        }
    }
    *curr = inode->inode_page[curr->inode_t.page];
    if (curr->ipage < pfs->inode_start_page || curr->inode_t.bank >= pfs->banks || curr->inode_t.page == 0 || (s8)curr->inode_t.page < 0) {
        if (curr->ipage == 1) {
            return 5;
        }
        return 3;
    }
    return 0;
}
s32 osPfsReadWriteFile_2(OSPfs *pfs, s32 file_no, u8 flag, int offset, int size_in_bytes, u8 *data_buffer) {
    s32 ret;
    __OSDir dir;
    __OSInode inode;
    u8 cbank;
    __OSInodeUnit curr_page;
    s32 offset_in_blocks;
    s32 size_in_blocks;
    u8 *buf;
    s32 addr;
    if (file_no >= pfs->dir_size || file_no < 0) {
        return 5;
    }
    if (size_in_bytes <= 0 || (size_in_bytes & 0x1F) != 0) {
        return 5;
    }
    if (offset < 0 || (offset & 0x1F) != 0) {
        return 5;
    }
    if ((pfs->status & 0x1) == 0) {
        return 5;
    }
    if (__osCheckId_2(pfs) == 2) {
        return 2;
    }
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = __osPfsSelectBank_2(pfs);
        if (ret != 0) {
            return ret;
        }
    }
    ret = __osContRamRead(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir);
    if (ret != 0) {
        return ret;
    }
    if (dir.company_code == 0 || dir.game_code == 0) {
        return 5;
    }
    if (dir.start_page.ipage < pfs->inode_start_page || dir.start_page.inode_t.bank >= pfs->banks || dir.start_page.inode_t.page == 0 || (s8)dir.start_page.inode_t.page < 0) {
        if (dir.start_page.ipage == 1) {
            return 5;
        }
        return 3;
    }
    if (flag == 0 && (dir.status & 2) == 0) {
        return 6;
    }
    cbank = 0xFF;
    offset_in_blocks = offset / 32;
    curr_page.ipage = dir.start_page.ipage;
    while (offset_in_blocks >= 8) {
        ret = next_page(pfs, &cbank, &inode, &curr_page);
        if (ret != 0) {
            return ret;
        }
        offset_in_blocks -= 8;
    }
    size_in_blocks = size_in_bytes / 32;
    buf = data_buffer;
    while (size_in_blocks > 0) {
        if (offset_in_blocks == 8) {
            ret = next_page(pfs, &cbank, &inode, &curr_page);
            if (ret != 0) {
                return ret;
            }
            offset_in_blocks = 0;
        }
        if (pfs->activebank != curr_page.inode_t.bank) {
            pfs->activebank = curr_page.inode_t.bank;
            ret = __osPfsSelectBank_2(pfs);
            if (ret != 0) {
                return ret;
            }
        }
        addr = curr_page.inode_t.page * 8 + offset_in_blocks;
        if (flag == 0) {
            ret = __osContRamRead(pfs->queue, pfs->channel, (u16)addr, buf);
        } else {
            ret = __osContRamWrite(pfs->queue, pfs->channel, (u16)addr, buf, 0);
        }
        if (ret != 0) {
            return ret;
        }
        buf += 32;
        size_in_blocks--;
        offset_in_blocks++;
    }
    if (flag == 1 && (dir.status & 2) == 0) {
        dir.status |= 2;
        pfs->activebank = 0;
        ret = __osPfsSelectBank_2(pfs);
        if (ret != 0) {
            return ret;
        }
        ret = __osContRamWrite(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir, 0);
        if (ret != 0) {
            return ret;
        }
    }
    return 0;
}
