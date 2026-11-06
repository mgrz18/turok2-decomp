/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/PFS/80447BB0_19.CPP, func_804486B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
extern s32 __osGetId_2(OSPfs *pfs);
extern s32 __osPfsSelectBank_2(OSPfs *pfs);
extern s32 __osContRamRead(void *queue, int channel, u16 address, u8 *buffer);
extern s32 __osCheckPackId_2(OSPfs *pfs, __OSPackId *temp);
extern s32 __osRepairPackId_2(OSPfs *pfs, __OSPackId *badid, __OSPackId *newid);
static __inline__ s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum) {
    u16 data = 0;
    u32 j;
    *csum = *icsum = 0;
    for (j = 0; j < 28; j += 2) {
        data = *(u16 *)((u32)ptr + j);
        *csum += data;
        *icsum += ~data;
    }
    return 0;
}
s32 __osGetId_2(OSPfs *pfs)
{
    int k;
    u16 sum;
    u16 isum;
    u8 temp[32];
    __OSPackId newid;
    s32 ret;
    __OSPackId *id;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = __osPfsSelectBank_2(pfs);
        if (ret != 0)
            return ret;
    }
    ret = __osContRamRead(pfs->queue, pfs->channel, 1, (u8 *)&temp);
    if (ret != 0)
        return ret;
    __osIdCheckSum((u16 *)&temp, &sum, &isum);
    id = (__OSPackId *)&temp;
    if (((__OSPackId *)temp)->checksum != sum || ((__OSPackId *)temp)->inverted_checksum != isum) {
        ret = __osCheckPackId_2(pfs, id);
        if (ret == 10) {
            ret = __osRepairPackId_2(pfs, id, &newid);
            if (ret != 0)
                return ret;
            id = &newid;
        } else if (ret != 0) {
            return ret;
        }
    }
    if ((id->deviceid & 1) == 0) {
        ret = __osRepairPackId_2(pfs, id, &newid);
        if (ret != 0)
            return ret;
        id = &newid;
        if ((newid.deviceid & 1) == 0) {
            return 11;
        }
    }
    for (k = 0; k < 32; k++) {
        pfs->id[k] = ((u8 *)id)[k];
    }
    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = pfs->banks * 2 + 3;
    pfs->dir_size = 16;
    pfs->inode_table = 8;
    pfs->minode_table = (pfs->banks + 1) * 8;
    pfs->dir_table = pfs->minode_table + pfs->banks * 8;
    ret = __osContRamRead(pfs->queue, pfs->channel, 7, pfs->label);
    if (ret != 0)
        return ret;
    return 0;
}
