/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040B8E8.c, func_0040B8E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char D_800B5168[];
extern char D_800B51C4[];
extern char D_800B5200[];
extern char D_800B5250[];
extern char D_800B52B0[];
s32 PakConfirmMenu_UpdateLine4_2(MenuItem *item) {
    switch (gPakConfirmKind) {
    default:
    case 0:
        item->text = D_800B5168;
        break;
    case 1:
        item->text = D_800B51C4;
        break;
    case 2:
        item->text = D_800B5200;
        break;
    case 3:
        item->text = D_800B5250;
        break;
    case 4:
        item->text = D_800B52B0;
        break;
    }
    return 0;
}
