#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern s32 Menu_StepValue(void *, s32, s32, s32, s32, s32);
s32 TeamBloodMenu_StepTimeLimit(void *first, void *second) {
    GameSettings *opts = &D_8011AAD8;
    opts->teamTimeLimit = Menu_StepValue(second, opts->teamTimeLimit, 5, 0, 30, 1);
    return 0;
}
