#include "common.h"
#include "structs/cinema.h"

/* The cinema state: a path playing between two times. */

extern Cinema *D_800C1BB0;

s32 Cinema_PathPlaying(void) {
    f32 temp_f1;
    s32 var_a0;

    var_a0 = 0;
    if (D_800C1BB0->active != 0) {
        temp_f1 = D_800C1BB0->time;
        if ((D_800C1BB0->startTime <= temp_f1) && (temp_f1 <= D_800C1BB0->endTime)) {
            var_a0 = 1;
        }
    }
    return var_a0;
}
