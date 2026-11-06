#include "common.h"
#include "structs/controller.h"
#include "globals/controller.h"

void Controller_Latch(ControllerProfile *ctl);


void Controller_LatchAll(void) {
    ControllerProfile *var_s0;
    s32 var_s1;

    if (gControllersInitialised != 0) {
        var_s1 = 0;
        var_s0 = gControllers;
        do {
            Controller_Latch(var_s0);
            var_s1 += 1;
            var_s0++;
        } while (var_s1 < 4);
        Controller_Latch(&gMergedController);
    }
}
