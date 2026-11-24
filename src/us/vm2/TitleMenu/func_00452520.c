#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"


void Controller_WatchPak(s32 on);

s32 SingleMenu_NewGame_2(s32 arg0, Menu *arg1) {
    Controller_WatchPak(arg1->controller->port);
    MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakMsgRumblePakBDesc_2, arg1->player, (sptr) arg1->controller, 0);
    return 1;
}
