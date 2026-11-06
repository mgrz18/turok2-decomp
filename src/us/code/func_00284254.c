#include "common.h"
#include "structs/cinema.h"

void Cinema_UpdateCamera();                               /* extern */
extern Cinema *D_800C1BB0;

void Cinema_UpdateCameraIfActive(void) {
    if (D_800C1BB0->active != 0) {
        Cinema_UpdateCamera();
    }
}
