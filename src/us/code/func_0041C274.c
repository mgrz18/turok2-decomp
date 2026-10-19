/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_8044472C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char D_800B3FC0[];
extern u8 D_8011AAEF;
extern char D_800B3FA8[];
extern char D_800B3F90[];
s32 func_0041C274(struct FieldRecAt14 *field) {
    switch (D_8011AAEF) {
    case 0:
        field->text = D_800B3F90;
        break;
    case 1:
        field->text = D_800B3FA8;
        break;
    case 2:
        field->text = D_800B3FC0;
        break;
    default:
        return 0;
    }
    return 0;
}
