#include "common.h"
#include "structs/menu.h"

void Controller_WatchPak(s8);                             /* extern */
extern s32 D_800C2020;

void func_004181C8(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    D_800C2020 = 0;
}
