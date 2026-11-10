#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

s32 Cinema_DrawFade();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Cinema_DrawFade receives Cinema_DrawOverlay's own argument
 * registers (pass-through); the host passes them. */
void Cinema_DrawOverlay(void * pt0) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_DrawFade(pt0);
    }
}
#else
void Cinema_DrawOverlay(void) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_DrawFade();
    }
}
#endif
