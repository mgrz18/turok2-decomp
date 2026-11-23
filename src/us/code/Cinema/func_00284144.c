#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"


s32 Cinema_IsSkippable(void) {
    return gCinemaPlayerP->scriptId == 0x50;
}
