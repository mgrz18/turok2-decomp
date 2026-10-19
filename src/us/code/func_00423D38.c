/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040B868_de_10.c, func_8040BBC0_us.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 D_801309B0;
extern char D_800B52DC[];
extern char D_800B5310[];
s32 func_00423D38(struct FieldRecAt14 *field) {
    u32 state = D_801309B0;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_3;
        case 2: goto case_3;
        case 3: goto case_1;
        case 4: goto case_2;
        }
state_other:
    field->text = D_800B5310;
    goto done;
case_1:
    field->text = D_800B5310;
    goto done;
case_2:
    field->text = D_800B5310;
    goto done;
case_3:
    field->text = D_800B52DC;
done:
    return 0;
}
