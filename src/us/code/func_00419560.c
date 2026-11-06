#include "common.h"
#include "structs/menu.h"

s32 Pak_ProbeSlot(s32);

s32 Controller_WatchPak(s8);                          /* extern */

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void SingleMenu_Init(Menu *arg0) {
    s8 temp_s0;

    temp_s0 = arg0->controller->port;
    Controller_WatchPak(temp_s0);
    Pak_ProbeSlot((s32) temp_s0);
}
