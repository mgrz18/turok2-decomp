/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_8.c, func_802457E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_002841D0(void);
s32 func_002841D0(void) {
    extern func_802457D0_S1 *D_800C1BB0;
    if (D_800C1BB0->unk38 != 0) {
        if (D_800C1BB0->unk1C > D_800C1BB0->unk34) {
            return 1;
        }
        return 0;
    }
    return 1;
}
