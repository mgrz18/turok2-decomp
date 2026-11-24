/* Second-module copy of MissionMenu_UpdateObjective2 (first module; this one at link address 0x0042501C), calling this module's own functions at their link addresses. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"


s32 MissionMenu_UpdateObjective2_2(MenuItem *item) {
    switch (sMissionHub) {
        case -1:
        case 0:
        default:
            item->text = sMissionMenuText_Space;
            break;
        case 1:
            item->text = sMissionMenuText_Children;
            break;
        case 2:
            item->text = sMissionMenuText_SistersOfDespair;
            break;
        case 3:
            item->text = sMissionMenuText_AmmoStorageFacilities;
            break;
        case 4:
            item->text = sMissionMenuText_Space;
            break;
        case 5:
            item->text = sMissionMenuText_MasterComputers;
            break;
        case 6:
            item->text = sMissionMenuText_AssemblyPlants;
            break;
    }
    return 0;
}
