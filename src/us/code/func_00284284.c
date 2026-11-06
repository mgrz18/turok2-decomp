#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

s32 Cinema_DrawFade();                            /* extern */

void Cinema_DrawOverlay(void) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_DrawFade();
    }
}
