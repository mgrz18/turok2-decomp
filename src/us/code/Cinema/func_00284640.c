#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"


s32 Cinema_GetActive(void) {
    return gCinemaPlayerP->active;
}
