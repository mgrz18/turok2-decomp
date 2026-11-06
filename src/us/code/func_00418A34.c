#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Options block; Turok 2 keeps this option as a signed byte at 0x20. */
extern s32 Menu_StepValue(void *, s32, s32, s32, s32, s32);
s32 BloodLustMenu_StepTimeLimit(void *first, void *second) {
    GameSettings *opts = &D_8011AAD8;
    opts->dmTimeLimit = Menu_StepValue(second, opts->dmTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}
