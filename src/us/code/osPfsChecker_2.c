/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */
/* From decompals/ultralib (libultra 2.0I): src/io/pfschecker.c, osPfsChecker (module-2 copy osPfsChecker_2). */
#include "common.h"
#include "ultra_types.h"

#define PFS_READ 0
#define PFS_WRITE 1
#define PFS_ERR_NEW_PACK 2
#define PFS_ERR_INCONSISTENT 3
#define PFS_CORRUPTED 2
#define PFS_EOF 1
#define PFS_PAGE_NOT_USED 3
#define DIR_STATUS_EMPTY 0

#define ERRCK(fn) \
    ret = fn;     \
    if (ret != 0) \
        return ret

#define SET_ACTIVEBANK_TO_ZERO()                 \
    if (pfs->activebank != 0) {                  \
        pfs->activebank = 0;                     \
        ERRCK(__osPfsSelectBank_2(pfs));               \
    }

#define CHECK_IPAGE(p)                                                                                        \
    (((p).ipage >= pfs->inode_start_page) && ((p).inode_t.bank < pfs->banks) && ((p).inode_t.page >= 0x01) && \
     ((p).inode_t.page < 0x80))

extern s32 __osCheckId_2(OSPfs *pfs); /* __osCheckId */
extern s32 __osGetId_2(OSPfs *pfs); /* __osGetId */
extern s32 __osPfsSelectBank_2(OSPfs *pfs); /* __osPfsSelectBank */
extern s32 __osPfsRWInode_2(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank); /* __osPfsRWInode */
extern s32 __osContRamRead(OSMesgQueue *mq, int channel, u16 address, u8 *buffer); /* __osContRamRead */
extern s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force); /* __osContRamWrite */
extern s32 corrupted_init_2(OSPfs *pfs, __OSInodeCache *cache); /* corrupted_init */
extern s32 corrupted_2(OSPfs *pfs, __OSInodeUnit fpage, __OSInodeCache *cache); /* corrupted */

s32 osPfsChecker_2(OSPfs *pfs) {
    int j;
    s32 ret;
    __OSInodeUnit next_page;
    __OSInode checked_inode;
    __OSInode tmp_inode;
    __OSDir tmp_dir;
    __OSInodeUnit file_next_node[16];
    __OSInodeCache cache;
    int fixed = 0;
    u8 bank;
    s32 cc;
    s32 cl;
    int offset;

    ret = __osCheckId_2(pfs);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = __osGetId_2(pfs);
    }

    if (ret != 0) {
        return ret;
    }

    ERRCK(corrupted_init_2(pfs, &cache));

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir));

        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0) {
            next_page = tmp_dir.start_page;
            cl = cc = 0;
            bank = 255;

            while (CHECK_IPAGE(next_page)) {
                if (bank != next_page.inode_t.bank) {
                    bank = next_page.inode_t.bank;
                    ret = __osPfsRWInode_2(pfs, &tmp_inode, PFS_READ, bank);
                    if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
                        return ret;
                    }
                }

                if ((cc = corrupted_2(pfs, next_page, &cache) - cl) != 0) {
                    break;
                }

                cl = 1;
                next_page = tmp_inode.inode_page[next_page.inode_t.page];
            }

            if (cc != 0 || next_page.ipage != PFS_EOF) {
                tmp_dir.company_code = 0;
                tmp_dir.game_code = 0;
                tmp_dir.start_page.ipage = 0;
                tmp_dir.status = DIR_STATUS_EMPTY;
                tmp_dir.data_sum = 0;

                SET_ACTIVEBANK_TO_ZERO();
                ERRCK(__osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir, 0));
                fixed++;
            }
        } else {
            if (tmp_dir.company_code != 0 || tmp_dir.game_code != 0) {
                tmp_dir.company_code = 0;
                tmp_dir.game_code = 0;
                tmp_dir.start_page.ipage = 0;
                tmp_dir.status = DIR_STATUS_EMPTY;
                tmp_dir.data_sum = 0;

                SET_ACTIVEBANK_TO_ZERO();
                ERRCK(__osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir, 0));
                fixed++;
            }
        }
    }

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir));

        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0 &&
            tmp_dir.start_page.ipage >= (u16)pfs->inode_start_page) {
            file_next_node[j].ipage = tmp_dir.start_page.ipage;
        } else {
            file_next_node[j].ipage = 0;
        }
    }

    for (bank = 0; bank < pfs->banks; bank++) {
        ret = __osPfsRWInode_2(pfs, &tmp_inode, PFS_READ, bank);

        if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
            return ret;
        }

        offset = (bank > 0) ? 1 : pfs->inode_start_page;

        for (j = 0; j < offset; j++) {
            checked_inode.inode_page[j].ipage = tmp_inode.inode_page[j].ipage;
        }

        for (; j < 128; j++) {
            checked_inode.inode_page[j].ipage = PFS_PAGE_NOT_USED;
        }

        for (j = 0; j < pfs->dir_size; j++) {
            while (file_next_node[j].inode_t.bank == bank &&
                   file_next_node[j].ipage >= (u16)pfs->inode_start_page) {
                u8 pp = file_next_node[j].inode_t.page;
                file_next_node[j] = checked_inode.inode_page[pp] = tmp_inode.inode_page[pp];
            }
        }
        ERRCK(__osPfsRWInode_2(pfs, &checked_inode, PFS_WRITE, bank));
    }

    if (fixed) {
        pfs->status |= PFS_CORRUPTED;
    } else {
        pfs->status &= ~PFS_CORRUPTED;
    }

    return 0;
}
