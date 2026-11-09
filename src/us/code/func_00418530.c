#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/menu.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

s32 FragTagMenu_StepTagLimit(void *first, void *second) {
    GameSettings *options = &gEngineApp_m_Settings;
    options->tagScoreLimit = Menu_StepValue(second, options->tagScoreLimit, 1, 0, 0x63, 1);
    return 0;
}
