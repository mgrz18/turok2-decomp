#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

/* De-faked in round 21 (j4): the matched draft wrapped the return in an if/else with two
 * identical arms; testing for the case that returns the field first gives the ROM's layout
 * (the field's path after the branch, the beq's delay slot empty). */

/* The playing cinema's first music override (0 when none plays). */
s32 Cinema_OverridesMusic1(void) {
    if (gCinemaPlayerP->active == 0) {
        return 0;
    }
    return gCinemaPlayerP->spec.unk40;
}
