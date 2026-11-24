/* Second-module copy of MissionMenu_UpdateLevelName (first module; this one at link address 0x00424EDC), calling this module's own functions at their link addresses. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"


s32 MissionMenu_UpdateLevelName_2(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = sMissionMenuText_Space;
            break;
        case 1:
            item->text = sMissionMenuText_PortOfAdia;
            break;
        case 2:
            item->text = sMissionMenuText_RiverOfSouls;
            break;
        case 3:
            item->text = sMissionMenuText_DeathMarshes;
            break;
        case 4:
            item->text = sMissionMenuText_LairOfTheBlindOnes;
            break;
        case 5:
            item->text = sMissionMenuText_HiveOfTheMantids;
            break;
        case 6:
            item->text = sMissionMenuText_PrimagensLightship;
            break;
    }
    return 0;
}
