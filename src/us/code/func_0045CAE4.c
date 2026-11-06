/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040B8E8.c, func_0040B8E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 gPakConfirmKind;
extern char D_800B5154[];
extern char D_800B51B0[];
extern char D_800B51EC[];
extern char D_800B5240[];
extern char D_800B529C[];
s32 PakConfirmMenu_UpdateLine3_2(MenuItem *item) {
    switch (gPakConfirmKind) {
    default:
    case 0:
        item->text = D_800B5154;
        break;
    case 1:
        item->text = D_800B51B0;
        break;
    case 2:
        item->text = D_800B51EC;
        break;
    case 3:
        item->text = D_800B5240;
        break;
    case 4:
        item->text = D_800B529C;
        break;
    }
    return 0;
}
