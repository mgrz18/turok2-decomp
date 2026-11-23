#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sBloodLustMenuText_LevelRandom[];
extern char sBloodLustMenuText_LevelExperimentata[];
extern char sBloodLustMenuText_LevelMoonlitMayhem[];
extern char sBloodLustMenuText_LevelCloseQuartered[];
extern char sBloodLustMenuText_LevelVulcansForge[];
extern char sBloodLustMenuText_LevelTeleportastic[];
extern char sBloodLustMenuText_LevelMirrororrim[];
extern char sBloodLustMenuText_LevelFishInABarrel[];
extern char sBloodLustMenuText_LevelH2Whoa[];
extern char sBloodLustMenuText_LevelEscheria[];
extern char sBloodLustMenuText_LevelMoshCrypt[];
extern char sBloodLustMenuText_LevelSquaresCubed[];
extern char sBloodLustMenuText_LevelCrazyEight[];
extern char sBloodLustMenuText_LevelBullseye[];

s32 TeamBloodMenu_LevelLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.teamArena) {
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
