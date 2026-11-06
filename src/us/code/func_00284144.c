#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

s32 Cinema_IsSkippable(void) {
    return gCinemaPlayerP->scriptId == 0x50;
}
