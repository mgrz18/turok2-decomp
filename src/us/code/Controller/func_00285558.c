#include "common.h"
#include "structs/controller.h"
#include "globals/controller.h"

/* What this reads of the controller state. */


s32 Cinema_IsActive(void);
void Sound__Play2D(s32);

/* Returns whether the buttons (START_BUTTON) were pressed; when they were and
 * gMenuSoundsEnabled is set and Cinema_IsActive returns 0, calls
 * Sound__Play2D(300). */
s32 Controller_StartPressedNoCinema(ControllerProfile *pad) {
    if (!(pad->buttonsSingle & 0x1000)) {
        return 0;
    }
    if (gMenuSoundsEnabled == 0) {
        return 1;
    }
    if (!Cinema_IsActive()) {
        Sound__Play2D(300);
    }
    return 1;
}
