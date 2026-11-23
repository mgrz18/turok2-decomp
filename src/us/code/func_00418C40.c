#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"


s32 BloodLustMenu_LevelLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.dmArena) {
    case 0:
        a->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        a->text = sBloodLustMenuText_LevelExperimentata;
        break;
    case 2:
        a->text = sBloodLustMenuText_LevelMoonlitMayhem;
        break;
    case 3:
        a->text = sBloodLustMenuText_LevelCloseQuartered;
        break;
    case 4:
        a->text = sBloodLustMenuText_LevelVulcansForge;
        break;
    case 5:
        a->text = sBloodLustMenuText_LevelTeleportastic;
        break;
    case 6:
        a->text = sBloodLustMenuText_LevelMirrororrim;
        break;
    case 7:
        a->text = sBloodLustMenuText_LevelFishInABarrel;
        break;
    case 8:
        a->text = sBloodLustMenuText_LevelH2Whoa;
        break;
    case 9:
        a->text = sBloodLustMenuText_LevelEscheria;
        break;
    case 10:
        a->text = sBloodLustMenuText_LevelMoshCrypt;
        break;
    case 11:
        a->text = sBloodLustMenuText_LevelSquaresCubed;
        break;
    case 12:
        a->text = sBloodLustMenuText_LevelCrazyEight;
        break;
    case 13:
        a->text = sBloodLustMenuText_LevelBullseye;
        break;
    }
    return 0;
}
