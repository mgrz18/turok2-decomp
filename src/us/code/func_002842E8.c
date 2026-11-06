#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"


void Cinema_ClearFinished(void) {
    gCinemaPlayerP->finished = 0;
}
