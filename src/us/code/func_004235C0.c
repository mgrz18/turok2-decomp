/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_6.c, func_8040A4A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char D_800B50D4[];
extern char D_800B509C[];
s32 DamagedPakMenu_UpdateLine2(struct FieldRecAt14 *field) {
    if (sDamagedPakInGame != 0) {
        field->text = D_800B509C;
    } else {
        field->text = D_800B50D4;
    }
    return 0;
}
