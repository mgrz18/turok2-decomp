#include "common.h"
#include "structs/menu.h"

void Controller_WatchPak(s8);                             /* extern */
extern s32 gDamagedPakContinued;

void func_004181C8(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    gDamagedPakContinued = 0;
}
