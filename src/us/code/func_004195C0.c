#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"


s32 Controller_WatchPak(s8);                          /* extern */

s32 SingleMenu_NewGame(s32 arg0, Menu *arg1) {
    Controller_WatchPak(arg1->controller->port);
    MenuPool_OpenMenu((s32) &gEngineApp_m_Game_m_MenuPool, (s32) &gPakMsgRumblePakBDesc, arg1->player, (s32) arg1->controller, 0);
    return 1;
}
