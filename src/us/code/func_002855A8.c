#include "common.h"
#include "structs/controller.h"

/* What this reads of the controller state. */

extern s32 D_800B6CF8;

void Sound__Play2D(s32);

/* Returns whether the buttons (START_BUTTON) were pressed; when they were and
 * D_800B6CF8 is set, calls Sound__Play2D(300). */
s32 Controller_StartPressed(ControllerProfile *pad) {
    if (!(pad->buttonsSingle & 0x1000)) {
        return 0;
    }
    if (D_800B6CF8 == 0) {
        return 1;
    }
    Sound__Play2D(300);
    return 1;
}
