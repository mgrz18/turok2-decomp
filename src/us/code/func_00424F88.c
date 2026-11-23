#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

extern s32 sMissionMenuText_MissionStatus;
extern s32 sMissionMenuText_MissionFailed;

s32 MissionMenu_UpdateTitle(MenuItem *arg0) {
    if (sMissionFailed != 0) {
        arg0->table = &sMissionMenuText_MissionFailed;
    } else {
        arg0->table = &sMissionMenuText_MissionStatus;
    }
    return 1;
}
