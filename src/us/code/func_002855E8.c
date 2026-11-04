#include "common.h"
#include "structs/controller.h"

/* What this reads of the controller state. */

extern s32 D_800B6CF8;

void func_00275624(s32);

/* Returns whether the buttons (A_BUTTON | B_BUTTON | START_BUTTON) were pressed; when they were and
 * D_800B6CF8 is set, calls func_00275624(300). */
s32 func_002855E8(ControllerProfile *pad) {
    if (!(pad->buttonsSingle & 0xD000)) {
        return 0;
    }
    if (D_800B6CF8 == 0) {
        return 1;
    }
    func_00275624(300);
    return 1;
}
