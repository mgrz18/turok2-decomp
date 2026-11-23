/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7E2C_eu_padding_A25BC_15.c, func_802B7AD8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void __osContGetInitData(u8 *valid, EntryRecAt3 *data) {
    extern char __osContBuffer; /* local type: globals/libultra.h declares it (OSPifRam); this file keeps its own view with the __osContNumControllers array below */
    extern u8 __osContNumControllers[]; /* local type: globals/libultra.h declares it (u8); as an array GCC 2 keeps its address in a register, as the ROM does */
    u8 *ptr;
    __OSContRequesFormat readformat;
    s32 i;
    u8 mask = 0;
    ptr = (u8 *)&__osContBuffer;
    for (i = 0; i < __osContNumControllers[0]; i++, ptr += sizeof(readformat), data++) {
        readformat = *(__OSContRequesFormat *)ptr;
        data->slot = (readformat.rxsize & 0xC0) >> 4;
        if (data->slot != 0) {
            continue;
        }
        data->id = (readformat.typel << 8) | readformat.typeh;
        data->team = readformat.status;
        mask |= 1 << i;
    }
    *valid = mask;
}
