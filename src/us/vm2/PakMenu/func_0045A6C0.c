#include "common.h"
#include "protos/pak_2.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "protos/pak.h"
#include "protos/pakmenu.h"



/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 PakMenu_PollPakB_2(Menu *arg0) {
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
                if (PakMenu_CheckPak_2((struct Menu *) arg0, var_a1, 0) != 0) {
                    gPakMenuLeaving = 1;
                }
            }
        }
    }
    return gPakMenuLeaving;
}
