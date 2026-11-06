#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

s32 Cinema_IsSkippable();                                /* extern */
s32 Cinema_IsActive();                                /* extern */

void Cinema_TrySkip(void) {
    if ((Cinema_IsActive() != 0) && (Cinema_IsSkippable() != 0) && (gCinemaPlayerP->fading == 0)) {
        gCinemaPlayerP->fading = 1;
        gCinemaPlayerP->fade = 1.0f;
    }
}
