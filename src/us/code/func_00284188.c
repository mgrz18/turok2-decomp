#include "common.h"
#include "structs/cinema.h"

/* The cinema state: a path playing between two times. */

extern Cinema *gCinemaPlayerP;

s32 Cinema_PathPlaying(void) {
    f32 temp_f1;
    s32 var_a0;

    var_a0 = 0;
    if (gCinemaPlayerP->active != 0) {
        temp_f1 = gCinemaPlayerP->time;
        if ((gCinemaPlayerP->startTime <= temp_f1) && (temp_f1 <= gCinemaPlayerP->endTime)) {
            var_a0 = 1;
        }
    }
    return var_a0;
}
