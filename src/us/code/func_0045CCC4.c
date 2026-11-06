/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040BB54.c, func_0040BB54.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 D_801309B0;
extern char D_800B5314[];
extern char D_800B52F8[];
s32 PakConfirmMenu_UpdateNo_2(MenuItem *item) {
    switch (D_801309B0) {
    default:
    case 0:
        item->text = D_800B5314;
        break;
    case 3:
        item->text = D_800B5314;
        break;
    case 4:
        item->text = D_800B5314;
        break;
    case 1:
    case 2:
        item->text = D_800B52F8;
        break;
    }
    return 0;
}
