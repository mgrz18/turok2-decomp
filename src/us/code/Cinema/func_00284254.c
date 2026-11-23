#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

void Cinema_UpdateCamera();                               /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Cinema_UpdateCamera receives Cinema_UpdateCameraIfActive's own argument
 * registers (pass-through); the host passes them. */
void Cinema_UpdateCameraIfActive(void * pt0) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_UpdateCamera(pt0);
    }
}
