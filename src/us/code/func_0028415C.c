#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

s32 Cinema_IsPending(void) {
    return ~gCinemaPlayerP->entryIndex != 0;
}
