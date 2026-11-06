#include "common.h"
#include "structs/menu.h"

s32 Controller_PakChanged(s32);
s32 PakMenu_CheckPak(s32, s32, s32);

extern s32 D_800C2028;
extern s32 D_80130990;
extern s32 D_801309A4;

s32 PakConfirmMenu_Poll(Menu *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (D_801309A4 == 0) {
        if (D_80130990 != 0) {
            var_v1 = D_800C2028;
        } else {
            var_v1 = (s32) arg0->controller->port;
        }
        if (var_v1 != -1) {
            if (D_80130990 != 0) {
                var_a0 = D_800C2028;
            } else {
                var_a0 = (s32) arg0->controller->port;
            }
            if (Controller_PakChanged(var_a0) != 0) {
                if (D_80130990 != 0) {
                    var_a1 = D_800C2028;
                } else {
                    var_a1 = (s32) arg0->controller->port;
                }
                if (PakMenu_CheckPak((s32) arg0, var_a1, 0) != 0) {
                    D_801309A4 = 1;
                }
            }
        }
    }
    return D_801309A4;
}
