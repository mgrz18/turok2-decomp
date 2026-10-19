/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446F60_de_11.c, func_80448BFC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
extern s32 func_00429EC4(OSPfs *pfs);
s32 func_00429DFC(OSPfs *pfs)
{
    int k;
    u8 temp[32];
    s32 ret;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = func_00429EC4(pfs);
        if (ret != 0)
            return ret;
    }
    ret = func_00299680(pfs->queue, pfs->channel, 1, temp);
    if (ret != 0) {
        if (ret != 2) {
            return ret;
        }
        ret = func_00299680(pfs->queue, pfs->channel, 1, temp);
        if (ret != 0)
            return ret;
    }
    for (k = 0; k < 32; k++) {
        if (pfs->id[k] != temp[k])
            return 2;
    }
    return 0;
}
