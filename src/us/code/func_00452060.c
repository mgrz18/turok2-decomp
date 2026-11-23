#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */


/* Names setting 0x2C. */
s32 func_00452060(MenuItem *item) {
    switch (gEngineApp_m_Settings.teamArena) {
    case 0:
        item->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        item->text = sBloodLustMenuText_LevelExperimentata;
        break;
    case 2:
        item->text = sBloodLustMenuText_LevelMoonlitMayhem;
        break;
    case 3:
        item->text = sBloodLustMenuText_LevelCloseQuartered;
        break;
    case 4:
        item->text = sBloodLustMenuText_LevelVulcansForge;
        break;
    case 5:
        item->text = sBloodLustMenuText_LevelTeleportastic;
        break;
    case 6:
        item->text = sBloodLustMenuText_LevelMirrororrim;
        break;
    case 7:
        item->text = sBloodLustMenuText_LevelFishInABarrel;
        break;
    case 8:
        item->text = sBloodLustMenuText_LevelH2Whoa;
        break;
    case 9:
        item->text = sBloodLustMenuText_LevelEscheria;
        break;
    case 10:
        item->text = sBloodLustMenuText_LevelMoshCrypt;
        break;
    case 11:
        item->text = sBloodLustMenuText_LevelSquaresCubed;
        break;
    case 12:
        item->text = sBloodLustMenuText_LevelCrazyEight;
        break;
    case 13:
        item->text = sBloodLustMenuText_LevelBullseye;
        break;
    }
    return 0;
}
