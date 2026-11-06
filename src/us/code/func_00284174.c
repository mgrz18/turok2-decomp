#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

s32 Cinema_IsActive(void) {
    return gCinemaPlayerP->active;
}
