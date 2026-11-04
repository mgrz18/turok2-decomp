#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Options block; Turok 2 keeps this option as a signed byte at 0x20. */
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_00418A34(void *first, void *second) {
    GameSettings *opts = &D_8011AAD8;
    opts->dmTimeLimit = func_0041648C(second, opts->dmTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}
