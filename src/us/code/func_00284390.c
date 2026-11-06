#include "common.h"
#include "structs/cinema.h"

f32 Cinema_GetFOV(f32);
extern Cinema *gCinemaPlayerP;

f32 Cinema_GetCameraFov(void) {
    f32 value;

    if (gCinemaPlayerP->active != 0) {
        value = Cinema_GetFOV(gCinemaPlayerP->time);
    } else {
        value = 47.5f;
    }
    return value;
}
