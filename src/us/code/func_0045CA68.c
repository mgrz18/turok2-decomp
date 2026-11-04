/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040B8E8.c, func_0040B8E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 D_801309B0;
extern char D_800B5140[];
extern char D_800B519C[];
extern char D_800B51D8[];
extern char D_800B5228[];
extern char D_800B5284[];
s32 func_0045CA68(MenuItem *item) {
    switch (D_801309B0) {
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
