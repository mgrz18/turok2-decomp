#include "common.h"
#include "globals/game.h"

s32 func_0045D44C();                            /* extern */
extern u8 D_800C203B;
extern f32 sResolutionToggleDelay;

s32 OptionsMenu_ToggleResolution_2(void) {
    if (sResolutionToggleDelay == 0.0f) {
        func_0045D44C();
        sResolutionToggleDelay = 18.0f;
        gEngineApp_m_Settings.unk180 = D_800C203B;
    }
    return 0;
}
