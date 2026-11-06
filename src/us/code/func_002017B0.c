/* WRONG CUT: splat's RNC_UnpackMethod2 (0x2010C0, 0x714 bytes) is not one C function.
 * 0x2010C0-0x2017A8 is hand-written asm (the decompressor next to the RNC code:
 * `addi`, every register saved, jalr to 0x201168 inside the block) and stays asm;
 * 0x2017A8-0x2017B0 is the ASCII padding "meSkyLay"; the only C here is the
 * object that starts at 0x2017B0 (size 0x24), written below as CISet__GetDataSize. */
#include "common.h"

void CISet__GetSetAndSize(s32 *arg0, s32 **arg1, s32 *arg2);

/* Returns the word that follows the table at `data` (count at data[0]). */
s32 CISet__GetDataSize(s32 *data) {
    s32 *table;
    s32 value;

    CISet__GetSetAndSize(data, &table, &value);
    return value;
}
