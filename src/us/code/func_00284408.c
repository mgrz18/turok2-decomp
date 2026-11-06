#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

s32 Cinema_GetScriptId(void) {
    return gCinemaPlayerP->scriptId;
}
