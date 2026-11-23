#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"


s32 MissionMenu_UpdateTitle(MenuItem *arg0) {
    if (sMissionFailed != 0) {
        arg0->table = &sMissionMenuText_MissionFailed;
    } else {
        arg0->table = &sMissionMenuText_MissionStatus;
    }
    return 1;
}
