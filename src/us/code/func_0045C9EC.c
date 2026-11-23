/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040B8E8.c, func_0040B8E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 PakConfirmMenu_UpdateLine1_2(MenuItem *item) {
    switch (gPakConfirmKind) {
    default:
    case 0:
        item->text = sPakConfirmMenuText_AttemptRepair;
        break;
    case 1:
        item->text = sPakConfirmMenuText_Error_B5194;
        break;
    case 2:
        item->text = sPakConfirmMenuText_Error_B51D0;
        break;
    case 3:
        item->text = sPakConfirmMenuText_Overwrite;
        break;
    case 4:
        item->text = sPakConfirmMenuText_Error_B527C;
        break;
    }
    return 0;
}
