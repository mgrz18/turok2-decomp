#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"


f32 Cinema_GetFarClip(void) {
    return gCinemaPlayerP->spec.scale * 10.24f;
}
