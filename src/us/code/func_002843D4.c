#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

f32 Cinema_GetFarClip(void) {
    return gCinemaPlayerP->spec.scale * 10.24f;
}
