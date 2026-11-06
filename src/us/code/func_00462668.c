/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/PFS/80447BB0_18.CPP, func_80448548_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
extern s32 __osPfsSelectBank_2(OSPfs *pfs);
extern s32 __osContRamRead(void *queue, int channel, u16 address, u8 *buffer);
extern s32 __osContRamWrite(void *queue, int channel, u16 address, u8 *buffer, int force);
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
s32 __osCheckPackId_2(OSPfs *pfs, __OSPackId *temp)
{
    u16 index[4];
    s32 ret = 0;
    u16 sum;
    u16 isum;
    int i;
    int j;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = __osPfsSelectBank_2(pfs);
        if (ret != 0)
            return ret;
    }
    index[0] = 1;
    index[1] = 3;
    index[2] = 4;
    index[3] = 6;
    for (i = 1; i < 4; i++) {
        ret = __osContRamRead(pfs->queue, pfs->channel, index[i], (u8 *)temp);
        if (ret != 0)
            return ret;
        __osIdCheckSum((u16 *)temp, &sum, &isum);
        if (temp->checksum == sum && temp->inverted_checksum == isum) {
            break;
        }
    }
    if (i == 4) {
        return 10;
    }
    for (j = 0; j < 4; j++) {
        if (j != i) {
            ret = __osContRamWrite(pfs->queue, pfs->channel, index[j], (u8 *)temp, 1);
            if (ret != 0)
                return ret;
        }
    }
    return 0;
}
