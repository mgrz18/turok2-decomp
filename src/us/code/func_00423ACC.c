/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040B868_de_4.c, func_8040B868_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char D_800B5130[];
extern char D_800B5194[];
extern char D_800B51D0[];
extern char D_800B521C[];
extern char D_800B527C[];
s32 func_00423ACC(struct FieldRecAt14 *field) {
    extern u32 D_801309B0;
    u32 state = D_801309B0;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto state_1;
        case 2: goto state_2;
        case 3: goto state_3;
        case 4: goto state_4;
        }
state_other:
    field->text = D_800B5130;
    goto done;
state_1:
    field->text = D_800B5194;
    goto done;
state_2:
    field->text = D_800B51D0;
    goto done;
state_3:
    field->text = D_800B521C;
    goto done;
state_4:
    field->text = D_800B527C;
done:
    return 0;
}
