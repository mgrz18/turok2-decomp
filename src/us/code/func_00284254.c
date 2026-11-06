#include "common.h"
#include "structs/cinema.h"

void Cinema_UpdateCamera();                               /* extern */
extern Cinema *gCinemaPlayerP;

void Cinema_UpdateCameraIfActive(void) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_UpdateCamera();
    }
}
