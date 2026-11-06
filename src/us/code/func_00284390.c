#include "common.h"
#include "structs/cinema.h"

f32 Cinema_GetFOV(f32);
extern Cinema *D_800C1BB0;

f32 Cinema_GetCameraFov(void) {
    f32 value;

    if (D_800C1BB0->active != 0) {
        value = Cinema_GetFOV(D_800C1BB0->time);
    } else {
        value = 47.5f;
    }
    return value;
}
