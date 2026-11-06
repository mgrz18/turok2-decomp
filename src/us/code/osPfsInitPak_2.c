/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */
/* From decompals/ultralib (libultra 2.0I): src/io/pfsinitpak.c, osPfsInitPak. */
#include "common.h"
#include "ultra_types.h"
#define PFS_INITIALIZED 1
#define PFS_ERR_DEVICE 11

extern void __osSiGetAccess(void); /* __osSiGetAccess */
extern void __osSiRelAccess(void); /* __osSiRelAccess */
extern s32 func_00427EA0(OSMesgQueue *queue, int channel); /* __osPfsGetStatus */
extern s32 func_00429DE4(OSPfs *pfs); /* __osPfsSelectBank */
extern s32 __osContRamRead(OSMesgQueue *mq, int channel, u16 address, u8 *buffer); /* __osContRamRead */
extern s32 func_00429CD4(u16 *ptr, u16 *csum, u16 *icsum); /* __osIdCheckSum */
extern s32 func_00429668(OSPfs *pfs, __OSPackId *temp); /* __osCheckPackId */
extern s32 func_00429380(OSPfs *pfs, __OSPackId *badid, __OSPackId *newid); /* __osRepairPackId */
extern s32 func_00428250(OSPfs *pfs); /* osPfsChecker */

s32 osPfsInitPak_2(OSMesgQueue *queue, OSPfs *pfs, int channel) {
    int k;
    s32 ret = 0;
    u16 sum;
    u16 isum;
    u8 temp[32];
    __OSPackId *id;
    __OSPackId newid;

    __osSiGetAccess();
    ret = func_00427EA0(queue, channel);
    __osSiRelAccess();
    if (ret != 0) {
        return ret;
    }

    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = 0;

    pfs->activebank = 0;
    if ((ret = func_00429DE4(pfs)) != 0) {
        return ret;
    }
    if ((ret = __osContRamRead(pfs->queue, pfs->channel, 1, temp)) != 0) {
        return ret;
    }

    func_00429CD4((u16 *)temp, &sum, &isum);
    id = (__OSPackId *)temp;

    if (id->checksum != sum || id->inverted_checksum != isum) {
        ret = func_00429668(pfs, id);
        if (ret != 0) {
            return ret;
        } else if (ret != 0) { /* duplicated check, as in 2.0I */
            return ret;
        }
    }

    if (!(id->deviceid & 1)) {
        ret = func_00429380(pfs, id, &newid);
        if (ret != 0) {
            return ret;
        }
        id = &newid;
        if (!(id->deviceid & 1)) {
            return PFS_ERR_DEVICE;
        }
    }

    for (k = 0; k < 32; k++) {
        pfs->id[k] = ((u8 *)id)[k];
    }

    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + 2 + (2 * pfs->banks);
    pfs->dir_size = 2 * 8;
    pfs->inode_table = 1 * 8;
    pfs->minode_table = (1 + pfs->banks) * 8;
    pfs->dir_table = pfs->minode_table + (pfs->banks * 8);

    if ((ret = __osContRamRead(pfs->queue, pfs->channel, 7, pfs->label)) != 0) {
        return ret;
    }

    ret = func_00428250(pfs);
    pfs->status |= PFS_INITIALIZED;
    return ret;
}
