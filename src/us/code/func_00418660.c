#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"


/* Label of the level item. */
s32 FragTagMenu_LevelLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.tagArena) {
    case 0:
        a->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        a->text = sFragTagMenuText_LevelMadhouse;
        break;
    case 2:
        a->text = sFragTagMenuText_LevelSnap;
        break;
    case 3:
        a->text = sFragTagMenuText_LevelSneaksNLadders;
        break;
    case 4:
        a->text = sFragTagMenuText_LevelPrimattica;
        break;
    case 5:
        a->text = sFragTagMenuText_LevelCrossfire;
        break;
    case 6:
        a->text = sFragTagMenuText_LevelYNGWTSF;
        break;
    }
    return 0;
}
