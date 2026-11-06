#include "common.h"
#include "structs/cinema.h"

s32 Cinema_IsSkippable();                                /* extern */
s32 Cinema_IsActive();                                /* extern */
extern Cinema *gCinemaPlayerP;

void Cinema_TrySkip(void) {
    if ((Cinema_IsActive() != 0) && (Cinema_IsSkippable() != 0) && (gCinemaPlayerP->fading == 0)) {
        gCinemaPlayerP->fading = 1;
        gCinemaPlayerP->fade = 1.0f;
    }
}
