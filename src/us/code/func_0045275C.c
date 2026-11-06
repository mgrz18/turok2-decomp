#include "common.h"
#include "structs/game.h"
#include "globals/menu_desc.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EBE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern void MenuPool_OpenChildMenu_2(void *, void *, void *);

s32 TitleMenu_Multi_2(void *first, void *second, void *third) {
    GameSettings *options = &gEngineApp_m_Settings;
    s32 i;
    options->multiplayer = 1;
    options->playerCount = 4;
    for (i = 0; i < 4; i++) {
        options->slots[i].m_bActive = 0;
    }
    MenuPool_OpenChildMenu_2(third, second, &gDeathmatchMenuDesc_2);
    return 1;
}
