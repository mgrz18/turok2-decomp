#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

s32 Cinema_GetActive(void) {
    return gCinemaPlayerP->active;
}
