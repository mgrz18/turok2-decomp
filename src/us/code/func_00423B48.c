/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040B868_de_4.c, func_8040B868_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char sPakConfirmMenuText_PreviouslySaved[];
extern char sPakConfirmMenuText_NotEnoughNotesOr_B519C[];
extern char sPakConfirmMenuText_NotEnoughNotesOr_B51D8[];
extern char sPakConfirmMenuText_YouWillBeDeleting[];
extern char sPakConfirmMenuText_ThisNoteIsCorrupt[];
s32 PakConfirmMenu_UpdateLine2(struct FieldRecAt14 *field) {
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
    field->text = sPakConfirmMenuText_PreviouslySaved;
    goto done;
state_1:
    field->text = sPakConfirmMenuText_NotEnoughNotesOr_B519C;
    goto done;
state_2:
    field->text = sPakConfirmMenuText_NotEnoughNotesOr_B51D8;
    goto done;
state_3:
    field->text = sPakConfirmMenuText_YouWillBeDeleting;
    goto done;
state_4:
    field->text = sPakConfirmMenuText_ThisNoteIsCorrupt;
done:
    return 0;
}
