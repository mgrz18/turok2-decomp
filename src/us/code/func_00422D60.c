/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_31.c, func_8040ACA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char D_800B506C[];
extern char D_800B500C[];
extern char D_800B4F94[];
extern char D_800B4C3C[];
extern char D_800B4FD0[];
extern char D_800B4DE0[];
extern char D_800B4D14[];
extern char D_800B4E6C[];
extern char D_800B4EEC[];
extern char D_800B4F18[];
extern char D_800B4F64[];
extern char D_800B4D7C[];
extern char D_800B4E34[];
extern char D_800B4BF8[];
extern char D_800B4E8C[];
extern char D_800B4CA8[];
extern u32 D_801309C0;
s32 func_00422D60(struct FieldRecAt14 *field) {
    u32 state = D_801309C0;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_1;
        case 2: goto case_2;
        case 3: goto case_3;
        case 4: goto case_4;
        case 5: goto case_5;
        case 6: goto case_6;
        case 7: goto case_7;
        case 8: goto case_8;
        case 9: goto case_9;
        case 10: goto case_10;
        case 11: goto case_11;
        case 12: goto case_12;
        case 13: goto case_13;
        case 14: goto case_14;
        case 15: goto case_15;
        }
state_other:
    field->text = D_800B4BF8;
    goto done;
case_1:
    field->text = D_800B4C3C;
    goto done;
case_2:
    field->text = D_800B4CA8;
    goto done;
case_3:
    field->text = D_800B4D7C;
    goto done;
case_4:
    field->text = D_800B4D14;
    goto done;
case_5:
    field->text = D_800B4DE0;
    goto done;
case_6:
    field->text = D_800B4E34;
    goto done;
case_7:
    field->text = D_800B4E6C;
    goto done;
case_8:
    field->text = D_800B4E8C;
    goto done;
case_9:
    field->text = D_800B4EEC;
    goto done;
case_10:
    field->text = D_800B4F18;
    goto done;
case_11:
    field->text = D_800B4F64;
    goto done;
case_12:
    field->text = D_800B4F94;
    goto done;
case_13:
    field->text = D_800B4FD0;
    goto done;
case_14:
    field->text = D_800B500C;
    goto done;
case_15:
    field->text = D_800B506C;
done:
    return 0;
}
