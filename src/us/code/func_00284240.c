#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"


s32 Cinema_IsFinished(void) {
    return gCinemaPlayerP->finished;
}
