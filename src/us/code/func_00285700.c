/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80263760_7.CPP, func_802643D0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/controller.h"

/* ControllerProfile profile, stride 0x224. */

void Sound__Play2D(s32 sound);

extern s32 gMenuSoundsEnabled; /* menu sounds enabled */

s32 Controller_MenuDown(ControllerProfile *ctrl) {
    if (ctrl->buttonsRepeat & 0x80404) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12D);
        }
        return 1;
    }
    return 0;
}
