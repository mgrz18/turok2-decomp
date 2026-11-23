/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040B8E8.c, func_0040B8E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 PakConfirmMenu_UpdateLine2_2(MenuItem *item) {
    switch (gPakConfirmKind) {
    default:
    case 0:
        item->text = sPakConfirmMenuText_PreviouslySaved;
        break;
    case 1:
        item->text = sPakConfirmMenuText_NotEnoughNotesOr_B519C;
        break;
    case 2:
        item->text = sPakConfirmMenuText_NotEnoughNotesOr_B51D8;
        break;
    case 3:
        item->text = sPakConfirmMenuText_YouWillBeDeleting;
        break;
    case 4:
        item->text = sPakConfirmMenuText_ThisNoteIsCorrupt;
        break;
    }
    return 0;
}
