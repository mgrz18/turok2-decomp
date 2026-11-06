#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 Controller_WatchPak(s8);                          /* extern */

void func_00419594(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    gDamagedPakContinued = 0;
}
