#include "common.h"
#include "structs/menu.h"

s32 Controller_WatchPak(s8);                          /* extern */
extern s32 D_800C2020;

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void func_00451178(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    D_800C2020 = 0;
}
