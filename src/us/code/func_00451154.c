#include "common.h"
#include "structs/menu.h"

s32 Controller_WatchPak(s8);                          /* extern */

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void CharacterMenu_Init_2(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
}
