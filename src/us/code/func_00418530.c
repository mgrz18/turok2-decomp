#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_00418530(void *first, void *second) {
    GameSettings *options = &D_8011AAD8;
    options->tagScoreLimit = func_0041648C(second, options->tagScoreLimit, 1, 0, 0x63, 1);
    return 0;
}
