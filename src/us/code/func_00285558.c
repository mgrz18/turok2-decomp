#include "common.h"
#include "structs/controller.h"

/* What this reads of the controller state. */

extern s32 D_800B6CF8;

s32 func_00284174(void);
void func_00275624(s32);

/* Returns whether the buttons (START_BUTTON) were pressed; when they were and
 * D_800B6CF8 is set and func_00284174 returns 0, calls
 * func_00275624(300). */
s32 func_00285558(ControllerProfile *pad) {
    if (!(pad->buttonsSingle & 0x1000)) {
        return 0;
    }
    if (D_800B6CF8 == 0) {
        return 1;
    }
    if (!func_00284174()) {
        func_00275624(300);
    }
    return 1;
}
