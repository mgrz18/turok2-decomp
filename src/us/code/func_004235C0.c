/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_6.c, func_8040A4A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char D_800B50D4[];
extern char D_800B509C[];
extern s32 D_80130964;
s32 func_004235C0(struct FieldRecAt14 *field) {
    if (D_80130964 != 0) {
        field->text = D_800B509C;
    } else {
        field->text = D_800B50D4;
    }
    return 0;
}
