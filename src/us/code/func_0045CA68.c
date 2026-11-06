/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040B8E8.c, func_0040B8E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char D_800B5140[];
extern char D_800B519C[];
extern char D_800B51D8[];
extern char D_800B5228[];
extern char D_800B5284[];
s32 PakConfirmMenu_UpdateLine2_2(MenuItem *item) {
    switch (gPakConfirmKind) {
    default:
    case 0:
        item->text = D_800B5140;
        break;
    case 1:
        item->text = D_800B519C;
        break;
    case 2:
        item->text = D_800B51D8;
        break;
    case 3:
        item->text = D_800B5228;
        break;
    case 4:
        item->text = D_800B5284;
        break;
    }
    return 0;
}
