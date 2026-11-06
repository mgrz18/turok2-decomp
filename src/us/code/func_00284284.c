#include "common.h"
#include "structs/cinema.h"

s32 Cinema_DrawFade();                            /* extern */
extern Cinema *D_800C1BB0;

void Cinema_DrawOverlay(void) {
    if (D_800C1BB0->active != 0) {
        Cinema_DrawFade();
    }
}
