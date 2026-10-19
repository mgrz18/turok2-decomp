/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_2.c, func_804464F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_004276F0(OSPfs *pfs, int page_no, u8 *data, u8 bank);
extern s32 func_00429EC4(OSPfs *pfs);
extern s32 func_00429EC4(OSPfs *pfs);
extern s32 func_00429EC4(OSPfs *pfs);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
s32 func_004276F0(OSPfs *pfs, int page_no, u8 *data, u8 bank)
{
    int i;
    s32 ret;
    ret = 0;
    pfs->activebank = bank;
    ret = func_00429EC4(pfs);
    if (ret != 0)
        return ret;
    for (i = 0; i < 8; i++) {
        ret = func_00299A40(pfs->queue, pfs->channel, page_no * 8 + i, data, 0);
        if (ret != 0) {
            break;
        }
    }
    pfs->activebank = 0;
    ret = func_00429EC4(pfs);
    return ret;
}
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
