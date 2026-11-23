#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"


extern u8 D_800B5C60[];
extern u8 D_800B5C6C[];

s32 StatsMenu_HeaderText(MenuItem *arg0) {
    if (gEngineApp_m_Settings.match.tag != 0) {
        arg0->text = (char *)D_800B5C6C;
    } else {
        arg0->text = (char *)D_800B5C60;
    }
    return 0;
}
