#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern s32 Menu_StepValue(void *, s32, s32, s32, s32, s32);
s32 BloodLustMenu_StepFragLimit(void *first, void *second) {
    GameSettings *options = &D_8011AAD8;
    options->dmScoreLimit = Menu_StepValue(second, options->dmScoreLimit, 1, 0, 0x63, 1);
    return 0;
}
