#include "common.h"
#include "structs/menu.h"

s32 func_0028591C(s32);
s32 func_0041DD90(s32, s32, s32);

extern s32 D_800C2028;
extern s32 D_80130990;
extern s32 D_801309BC;

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 func_004217A0(Menu *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;

    if (D_801309BC == 0) {
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
            if (func_0028591C(var_a0) != 0) {
                if (D_80130990 != 0) {
                    var_a1 = D_800C2028;
                } else {
                    var_a1 = (s32) arg0->controller->port;
                }
                if (func_0041DD90((s32) arg0, var_a1, 0) != 0) {
                    D_801309BC = 1;
                }
            }
        }
    }
    return D_801309BC;
}
