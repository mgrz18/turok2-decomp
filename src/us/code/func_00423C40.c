/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040B868_de_4.c, func_8040B868_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char sPakConfirmMenuText_WouldYouStillLike[];
extern char sPakConfirmMenuText_Save[];
extern char sPakConfirmMenuText_SaveAllNoteTypes[];
extern char sPakConfirmMenuText_WouldYouLikeTo_B5250[];
extern char sPakConfirmMenuText_WouldYouLikeTo_B52B0[];
s32 PakConfirmMenu_UpdateLine4(struct FieldRecAt14 *field) {
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
    field->text = sPakConfirmMenuText_WouldYouStillLike;
    goto done;
state_1:
    field->text = sPakConfirmMenuText_Save;
    goto done;
state_2:
    field->text = sPakConfirmMenuText_SaveAllNoteTypes;
    goto done;
state_3:
    field->text = sPakConfirmMenuText_WouldYouLikeTo_B5250;
    goto done;
state_4:
    field->text = sPakConfirmMenuText_WouldYouLikeTo_B52B0;
done:
    return 0;
}
