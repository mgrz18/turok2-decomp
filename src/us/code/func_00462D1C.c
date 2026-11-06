/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/PFS/80447BB0_20.CPP, func_80448BFC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
extern s32 __osCheckId_2(OSPfs *pfs);
extern s32 __osPfsSelectBank_2(OSPfs *pfs);
extern s32 __osContRamRead(void *queue, int channel, u16 address, u8 *buffer);
s32 __osCheckId_2(OSPfs *pfs)
{
    int k;
    u8 temp[32];
    s32 ret;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = __osPfsSelectBank_2(pfs);
        if (ret != 0)
            return ret;
    }
    ret = __osContRamRead(pfs->queue, pfs->channel, 1, temp);
    if (ret != 0) {
        if (ret != 2) {
            return ret;
        }
        ret = __osContRamRead(pfs->queue, pfs->channel, 1, temp);
        if (ret != 0)
            return ret;
    }
    for (k = 0; k < 32; k++) {
        if (pfs->id[k] != temp[k])
            return 2;
    }
    return 0;
}
