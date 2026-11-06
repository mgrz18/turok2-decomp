/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040B8E8.c, func_0040B8E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 D_801309B0;
extern char D_800B5180[];
extern char D_800B51CC[];
extern char D_800B5218[];
extern char D_800B5264[];
extern char D_800B52C4[];
s32 PakConfirmMenu_UpdateLine5_2(MenuItem *item) {
    switch (D_801309B0) {
    default:
    case 0:
        item->text = D_800B5180;
        break;
    case 1:
        item->text = D_800B51CC;
        break;
    case 2:
        item->text = D_800B5218;
        break;
    case 3:
        item->text = D_800B5264;
        break;
    case 4:
        item->text = D_800B52C4;
        break;
    }
    return 0;
}
