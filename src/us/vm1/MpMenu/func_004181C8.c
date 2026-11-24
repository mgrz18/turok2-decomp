#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern void Controller_WatchPak(s32 arg);

void CharacterMenu_InitAfterPak(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    gDamagedPakContinued = 0;
}
