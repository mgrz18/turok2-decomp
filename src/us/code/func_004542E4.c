/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043D5F0.c, func_0043D5F0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 D_8011AADC;
extern s32 D_8011AAD8;
extern char D_800B5854[];
extern char D_800B5870[];
s32 CheatMenu_UnlimitedAmmoText_2(MenuItem *a) {
    if (D_8011AADC & 0x8) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (D_8011AAD8 & 0x8) a->text = D_800B5854; else a->text = D_800B5870;
    return 0;
}
