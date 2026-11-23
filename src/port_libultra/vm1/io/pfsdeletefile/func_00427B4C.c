/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_2.c, func_8044694C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 __osPfsSelectBank(OSPfs *pfs);
extern s32 __osPfsSelectBank(OSPfs *pfs);
extern s32 __osBlockSum(OSPfs *pfs, u8 page_no, u16 *sum, u8 bank);
extern s32 __osPfsSelectBank(OSPfs *pfs);
extern u16 __osSumcalc(u8 *bytes, s32 count);
extern s32 __osContRamRead(void *queue, int channel, u16 address, u8 *buffer);
extern s32 __osContRamRead(void *queue, int channel, u16 address, u8 *buffer);
s32 __osBlockSum(OSPfs *pfs, u8 page_no, u16 *sum, u8 bank)
{
    int i;
    s32 ret;
    u8 data[32];
    ret = 0;
    pfs->activebank = bank;
    ret = __osPfsSelectBank(pfs);
    if (ret != 0)
        return ret;
    for (i = 0; i < 8; i++) {
        ret = __osContRamRead(pfs->queue, pfs->channel, page_no * 8 + i, data);
        if (ret != 0) {
            pfs->activebank = 0;
            __osPfsSelectBank(pfs);
            return ret;
        }
        *sum = *sum + __osSumcalc(data, sizeof(data));
    }
    pfs->activebank = 0;
    ret = __osPfsSelectBank(pfs);
    return ret;
}
