#include "common.h"
#include "structs/menu.h"

s32 MenuPool_OpenMenu(s32, s32, s32, s32, s32);

s32 Controller_WatchPak(s8);                          /* extern */
extern s32 D_0043BD44;
extern s32 D_80119E2C;

s32 SingleMenu_NewGame(s32 arg0, Menu *arg1) {
    Controller_WatchPak(arg1->controller->port);
    MenuPool_OpenMenu((s32) &D_80119E2C, (s32) &D_0043BD44, arg1->player, (s32) arg1->controller, 0);
    return 1;
}
