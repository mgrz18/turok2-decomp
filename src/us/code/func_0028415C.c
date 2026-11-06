#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"


s32 Cinema_IsPending(void) {
    return ~gCinemaPlayerP->entryIndex != 0;
}
