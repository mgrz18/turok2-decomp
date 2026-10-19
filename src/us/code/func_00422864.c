/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_30.c, func_8040AC54_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char D_800B4B9C[];
extern char D_800B4BA4[];
extern u32 D_801309C0;
s32 func_00422864(struct FieldRecAt14 *field) {
    u32 state = D_801309C0;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto state_other;
        case 2: goto state_other;
        case 3: goto state_other;
        case 4: goto state_other;
        case 5: goto case_1;
        case 6: goto case_1;
        case 7: goto state_other;
        case 8: goto state_other;
        case 9: goto case_1;
        case 10: goto case_1;
        case 11: goto state_other;
        case 12: goto case_1;
        case 13: goto state_other;
        case 14: goto state_other;
        case 15: goto case_1;
        }
state_other:
    field->text = D_800B4B9C;
    goto done;
case_1:
    field->text = D_800B4BA4;
done:
    return 0;
}
