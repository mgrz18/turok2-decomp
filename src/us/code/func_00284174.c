#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"


s32 Cinema_IsActive(void) {
    return gCinemaPlayerP->active;
}
