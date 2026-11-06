/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80263760_7.CPP, func_802643E8_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/controller.h"
#include "globals/controller.h"

/* ControllerProfile profile, stride 0x224. */

void Sound__Play2D(s32 sound);


s32 Controller_MenuSelect(ControllerProfile *ctrl) {
    if (ctrl->buttonsSingle & 0x8000) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12C);
        }
        return 1;
    }
    return 0;
}
