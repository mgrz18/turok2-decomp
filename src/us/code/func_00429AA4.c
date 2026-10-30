/* WRONG CUT: REAL func_00429AA4 size 0x2E0 (splat cut it into func_00429AA4 + func_00429CA4 + func_00429CD4 + func_00429D1C). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SDK/PFS/80447BB0_19.CPP, func_804488A4_de, with Turok 2's layouts. */
/* __osPfsRWInode (libultra contpfs.c), with __osSumcalc inlined. */
#include "rw/abu_types.h"

extern s32 func_00429EC4(OSPfs *pfs); /* __osPfsSelectBank */
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer); /* __osContRamRead */
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force); /* __osContRamWrite */

static __inline__ u16 sumcalc(u8 *ptr, int length) {
    int i;
    u32 sum;
    u8 *tmp;

    sum = 0;
    tmp = ptr;
    for (i = 0; i < length; i++) {
        sum += *tmp++;
        sum = sum & 0xFFFF;
    }
    return sum;
}

s32 func_00429AA4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank)
{
    u8 sum;
    int j;
    s32 ret;
    int offset;
    u8 *addr;

    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = func_00429EC4(pfs);
        if (ret != 0)
            return ret;
    }

    offset = (bank > 0) ? 1 : pfs->inode_start_page;

    if (flag == 1) {
        inode->inode_page[0].inode_t.page = sumcalc((u8 *)&inode->inode_page[offset], (128 - offset) * 2);
    }

    for (j = 0; j < 8; j++) {
        addr = ((u8 *)inode->inode_page + j * 32);

        if (flag == 1) {
            ret = func_00299A40(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr, 0);
            ret = func_00299A40(pfs->queue, pfs->channel, pfs->minode_table + bank * 8 + j, addr, 0);
        } else {
            ret = func_00299680(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr);
        }

        if (ret != 0) {
            return ret;
        }
    }

    if (flag == 0) {
        sum = sumcalc((u8 *)&inode->inode_page[offset], (128 - offset) * 2);
        if (sum != inode->inode_page[0].inode_t.page) {
            for (j = 0; j < 8; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = func_00299680(pfs->queue, pfs->channel, pfs->minode_table + bank * 8 + j, addr);
            }

            if (sum != inode->inode_page[0].inode_t.page) {
                return 3;
            }

            for (j = 0; j < 8; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = func_00299A40(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr, 0);
            }
        } else {
            for (j = 0; j < 8; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = func_00299A40(pfs->queue, pfs->channel, pfs->minode_table + bank * 8 + j, addr, 0);
            }
        }
    }

    return 0;
}
