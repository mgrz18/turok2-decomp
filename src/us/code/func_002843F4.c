#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

s32 Cinema_GetFogColor(void) {
    return gCinemaPlayerP->spec.unk48;
}
