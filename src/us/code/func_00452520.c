#include "common.h"
#include "structs/menu.h"

s32 MenuPool_OpenMenu_2(s32, s32, s32, s32, s32);

s32 Controller_WatchPak(s8);                          /* extern */
extern s32 D_0043BC64;
extern s32 D_80119E2C;

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 SingleMenu_NewGame_2(s32 arg0, Menu *arg1) {
    Controller_WatchPak(arg1->controller->port);
    MenuPool_OpenMenu_2((s32) &D_80119E2C, (s32) &D_0043BC64, (s32) arg1->player, (s32) arg1->controller, 0);
    return 1;
}
