/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80263760_7.CPP, func_802643B8_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/controller.h"
#include "globals/controller.h"

/* ControllerProfile profile, stride 0x224. */

void Sound__Play2D(s32 sound);


s32 Controller_MenuUp(ControllerProfile *ctrl) {
    if (ctrl->buttonsRepeat & 0x10808) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12D);
        }
        return 1;
    }
    return 0;
}
