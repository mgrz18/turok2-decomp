#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/menu.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
s32 TeamBloodMenu_StepFragLimit(void *first, void *second) {
    GameSettings *game = &gEngineApp_m_Settings;
    game->teamScoreLimit = Menu_StepValue(second, game->teamScoreLimit, 1, 0, 99, 1);
    return 0;
}
