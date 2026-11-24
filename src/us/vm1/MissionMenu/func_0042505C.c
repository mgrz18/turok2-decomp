#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"


s32 MissionMenu_UpdateObjective1(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = sMissionMenuText_Space;
            break;
        case 1:
            item->text = sMissionMenuText_DistressBeacons;
            break;
        case 2:
            item->text = sMissionMenuText_SoulGates;
            break;
        case 3:
            item->text = sMissionMenuText_Prisoners;
            break;
        case 4:
            item->text = sMissionMenuText_ThermalVents;
            break;
        case 5:
            item->text = sMissionMenuText_QueenEmbryos;
            break;
        case 6:
            item->text = sMissionMenuText_PowerGenerators;
            break;
    }
    return 0;
}
