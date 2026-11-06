#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

void Cinema_UpdateCamera();                               /* extern */

void Cinema_UpdateCameraIfActive(void) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_UpdateCamera();
    }
}
