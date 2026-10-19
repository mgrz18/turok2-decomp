/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_16.c, func_804098B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_0045A240(s32 *block);
s32 func_0045A240(s32 *block) {
    return ((struct Shape_typemap_3 *) (block[0] + (s32) block))[-1].field_0;
}
