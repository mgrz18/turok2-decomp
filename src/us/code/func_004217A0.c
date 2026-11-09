#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 Controller_PakChanged(s32);
s32 PakMenu_CheckPak(s32, s32, s32);


/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 PakMenu_PollPakB(Menu *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (gPakMenuLeaving == 0) {
        if (gPakMenuAnyPak != 0) {
            var_v1 = gPakMenuPort;
        } else {
            var_v1 = (s32) arg0->controller->port;
        }
        if (var_v1 != -1) {
            if (gPakMenuAnyPak != 0) {
                var_a0 = gPakMenuPort;
            } else {
                var_a0 = (s32) arg0->controller->port;
            }
            if (Controller_PakChanged(var_a0) != 0) {
                if (gPakMenuAnyPak != 0) {
                    var_a1 = gPakMenuPort;
                } else {
                    var_a1 = (s32) arg0->controller->port;
                }
                if (PakMenu_CheckPak((sptr) arg0, var_a1, 0) != 0) {
                    gPakMenuLeaving = 1;
                }
            }
        }
    }
    return gPakMenuLeaving;
}
