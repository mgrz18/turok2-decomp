/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040A500.c, func_0040A500.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

extern char D_800B49DC[];
extern char D_800B49F0[];
extern char D_800B4A04[];
s32 func_00421E78(MenuItem *item) {
    switch (D_800C201C->settings.difficulty) {
    default:
    case 0:
        item->text = D_800B49DC;
        break;
    case 16:
        item->text = D_800B49F0;
        break;
    case 32:
        item->text = D_800B4A04;
        break;
    }
    return 0;
}
