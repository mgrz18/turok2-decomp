#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

void Cinema_ClearFinished(void) {
    gCinemaPlayerP->finished = 0;
}
