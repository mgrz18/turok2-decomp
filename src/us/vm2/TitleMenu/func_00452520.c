#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"

s32 MenuPool_OpenMenu_2(s32, s32, sptr, sptr, sptr);

s32 Controller_WatchPak(s8);                          /* extern */

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 SingleMenu_NewGame_2(s32 arg0, Menu *arg1) {
    Controller_WatchPak(arg1->controller->port);
    MenuPool_OpenMenu_2((sptr) &gEngineApp_m_Game_m_MenuPool, (sptr) &gPakMsgRumblePakBDesc_2, (sptr) arg1->player, (sptr) arg1->controller, 0);
    return 1;
}
