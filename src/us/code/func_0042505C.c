#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

extern char D_800C1768[];
extern char sMissionMenuText_DistressBeacons[];
extern char sMissionMenuText_SoulGates[];
extern char sMissionMenuText_Prisoners[];
extern char sMissionMenuText_ThermalVents[];
extern char sMissionMenuText_QueenEmbryos[];
extern char sMissionMenuText_PowerGenerators[];

s32 MissionMenu_UpdateObjective1(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
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
