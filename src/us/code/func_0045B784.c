/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040ACD4.c, func_0040ACD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 gPakMessageKind;
extern char D_800B4B9C[];
extern char D_800B4BA4[];
s32 PakMessage_UpdateHeading_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = D_800B4B9C;
        break;
    case 5:
    case 6:
    case 9:
    case 10:
    case 12:
    case 15:
        item->text = D_800B4BA4;
        break;
    }
    return 0;
}
