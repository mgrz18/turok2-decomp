/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_6.c, func_8040A4A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu_text.h"
#include "globals/pak.h"

s32 DamagedPakMenu_UpdateContinue_2(struct FieldRecAt14 *field) {
    if (gPakMenuIsLoad != 0) {
        field->text = sDamagedPakMenuText_Continue;
    } else {
        field->text = sDamagedPakMenuText_ContinueWithoutSaving;
    }
    return 0;
}
