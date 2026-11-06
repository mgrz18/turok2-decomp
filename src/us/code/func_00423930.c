/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_6.c, func_8040A4A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char D_800B5118[];
extern char D_800B510C[];
s32 DamagedPakMenu_UpdateContinue(struct FieldRecAt14 *field) {
    if (gPakMenuIsLoad != 0) {
        field->text = D_800B510C;
    } else {
        field->text = D_800B5118;
    }
    return 0;
}
