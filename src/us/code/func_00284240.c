#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

s32 Cinema_IsFinished(void) {
    return gCinemaPlayerP->finished;
}
