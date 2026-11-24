#include "common.h"
#include "structs/menu.h"

void Controller_WatchPak(s32 on);

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void CharacterMenu_Init_2(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
}
