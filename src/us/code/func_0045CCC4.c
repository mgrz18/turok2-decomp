/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040BB54.c, func_0040BB54.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char sPakConfirmMenuText_No[];
extern char sPakConfirmMenuText_ContinueWithoutSaving[];
s32 PakConfirmMenu_UpdateNo_2(MenuItem *item) {
    switch (gPakConfirmKind) {
    default:
    case 0:
        item->text = sPakConfirmMenuText_No;
        break;
    case 3:
        item->text = sPakConfirmMenuText_No;
        break;
    case 4:
        item->text = sPakConfirmMenuText_No;
        break;
    case 1:
    case 2:
        item->text = sPakConfirmMenuText_ContinueWithoutSaving;
        break;
    }
    return 0;
}
