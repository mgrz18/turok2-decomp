#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

void Controller_WatchPak(s32 on);

void SingleMenu_InitAfterPak_2(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    gDamagedPakContinued = 0;
}
