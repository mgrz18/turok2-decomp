#include "common.h"
#include "structs/controller.h"

void Controller_Latch(ControllerProfile *ctl);

extern u8 D_800B6CF0;

void Controller_LatchAll(void) {
    ControllerProfile *var_s0;
    s32 var_s1;

    if (D_800B6CF0 != 0) {
        var_s1 = 0;
        var_s0 = D_800F5EC0;
        do {
            Controller_Latch(var_s0);
            var_s1 += 1;
            var_s0++;
        } while (var_s1 < 4);
        Controller_Latch(&D_800F5C78);
    }
}
