/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80400000_de_11.c, func_80403B0C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 func_00412E4C(u8 *bits, s32 index);
u32 func_00412E4C(u8 *bits, s32 index)
{
    s32 byte = index >> 3;
    u32 mask;
    index &= 7;
    mask = 1;
    bits += byte;
    return *bits & (mask << index);
}
