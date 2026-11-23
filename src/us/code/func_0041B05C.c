/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043C730.c, func_0043D2D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
extern u8 D_800C1144[];
extern u8 D_800B5B2C[], D_800B5B48[], D_800B5B64[], D_800B5B80[], D_800B5B9C[], D_800B5BB8[], D_800B5BD4[], D_800B5BF0[], D_800B5C0C[], D_800B5C28[], D_800B5C44[];
s32 CheatMenu_WarpText(MenuItem *o) {
    switch (sCheatsMenuSelection) {
    default: o->text = (char *)D_800C1144; break;
    case 0: o->text = (char *)D_800B5B2C; break;
    case 1: o->text = (char *)D_800B5B48; break;
    case 2: o->text = (char *)D_800B5B64; break;
    case 3: o->text = (char *)D_800B5B80; break;
    case 4: o->text = (char *)D_800B5B9C; break;
    case 5: o->text = (char *)D_800B5BB8; break;
    case 6: o->text = (char *)D_800B5BF0; break;
    case 7: o->text = (char *)D_800B5BD4; break;
    case 8: o->text = (char *)D_800B5C0C; break;
    case 9: o->text = (char *)D_800B5C28; break;
    case 10: o->text = (char *)D_800B5C44; break;
    }
    return 0;
}
