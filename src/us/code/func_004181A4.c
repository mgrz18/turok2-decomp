#include "common.h"
#include "structs/menu.h"

s32 Controller_WatchPak(s8);                          /* extern */

void CharacterMenu_Init(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
}
