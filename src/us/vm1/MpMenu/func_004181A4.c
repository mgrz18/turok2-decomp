#include "common.h"
#include "structs/menu.h"

extern void Controller_WatchPak(s32 arg);

void CharacterMenu_Init(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
}
