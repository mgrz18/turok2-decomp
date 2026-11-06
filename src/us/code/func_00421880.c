#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"


/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 PakMenu_PollStartPressed(Menu *arg0) {
    if ((arg0->controller->buttons & 0x1000) && (sPakMenuStartLatched == 0)) {
        gPakMenuStartPressed = 1;
        sPakMenuStartLatched = 1;
    }
    return gPakMenuStartPressed;
}
