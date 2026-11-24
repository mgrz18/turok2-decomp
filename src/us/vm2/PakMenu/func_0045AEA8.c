#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "structs/save.h"

s32 Level_GetHub(s32 id);


s32 GameNoteMenu_UpdateLevel_2(MenuItem *item) {
    switch (Level_GetHub(gLoadBuffer->player.m_CheckPointWarpID)) {
        case -1:
        default:
            item->text = sGameNoteMenuText_UnknownLevelId;
            break;
        case 0:
            item->text = sGameNoteMenuText_TheHub;
            break;
        case 1:
            item->text = sGameNoteMenuText_PortOfAdia;
            break;
        case 2:
            item->text = sGameNoteMenuText_RiverOfSouls;
            break;
        case 3:
            item->text = sGameNoteMenuText_DeathMarshes;
            break;
        case 4:
            item->text = sGameNoteMenuText_LairOfTheBlindOnes;
            break;
        case 5:
            item->text = sGameNoteMenuText_HiveOfTheMantids;
            break;
        case 6:
            item->text = sGameNoteMenuText_PrimagensLightship;
            break;
    }
    return 0;
}
