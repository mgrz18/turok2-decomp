#include "common.h"
#include "structs/cinema.h"

s32 Cinema_DrawFade();                            /* extern */
extern Cinema *gCinemaPlayerP;

void Cinema_DrawOverlay(void) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_DrawFade();
    }
}
