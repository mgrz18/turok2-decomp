#include "common.h"
#include "structs/cinema.h"

s32 Cinema_IsSkippable();                                /* extern */
s32 Cinema_IsActive();                                /* extern */
extern Cinema *D_800C1BB0;

void Cinema_TrySkip(void) {
    if ((Cinema_IsActive() != 0) && (Cinema_IsSkippable() != 0) && (D_800C1BB0->fading == 0)) {
        D_800C1BB0->fading = 1;
        D_800C1BB0->fade = 1.0f;
    }
}
