/* WRONG CUT: the real function starts at 0x0045AEA8 (size 0xD8 = 216): splat
 * cut its first two words (lui/lw gLoadBuffer) as GameNoteMenu_UpdateLevel_2. */
#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

/* Set the menu item's text from a state of the current object. */

extern char sGameNoteMenuText_UnknownLevelId[];
extern char sGameNoteMenuText_TheHub[];
extern char sGameNoteMenuText_PortOfAdia[];
extern char sGameNoteMenuText_RiverOfSouls[];
extern char sGameNoteMenuText_DeathMarshes[];
extern char sGameNoteMenuText_LairOfTheBlindOnes[];
extern char sGameNoteMenuText_HiveOfTheMantids[];
extern char sGameNoteMenuText_PrimagensLightship[];

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
