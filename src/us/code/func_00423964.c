#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 Controller_PakChanged(s32);
s32 PakMenu_CheckPak(s32, s32, s32);

extern s32 gPakMenuAnyPak;
extern s32 sPakConfirmDone;

s32 PakConfirmMenu_Poll(Menu *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (sPakConfirmDone == 0) {
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
                if (PakMenu_CheckPak((s32) arg0, var_a1, 0) != 0) {
                    sPakConfirmDone = 1;
                }
            }
        }
    }
    return sPakConfirmDone;
}
