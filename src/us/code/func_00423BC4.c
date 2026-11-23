/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040B868_de_4.c, func_8040B868_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char sPakConfirmMenuText_DataMayBeLost[];
extern char sPakConfirmMenuText_PagesAvailableTo_B51B0[];
extern char sPakConfirmMenuText_PagesAvailableTo_B51EC[];
extern char sPakConfirmMenuText_TheOldNote[];
extern char sPakConfirmMenuText_AndCannotBeUsed[];
s32 PakConfirmMenu_UpdateLine3(struct FieldRecAt14 *field) {
    u32 state = gPakConfirmKind;
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
    field->text = sPakConfirmMenuText_DataMayBeLost;
    goto done;
state_1:
    field->text = sPakConfirmMenuText_PagesAvailableTo_B51B0;
    goto done;
state_2:
    field->text = sPakConfirmMenuText_PagesAvailableTo_B51EC;
    goto done;
state_3:
    field->text = sPakConfirmMenuText_TheOldNote;
    goto done;
state_4:
    field->text = sPakConfirmMenuText_AndCannotBeUsed;
done:
    return 0;
}
