#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 func_0042452C(void);

extern f32 D_800C132C;
extern u8 D_800C203B;
extern f32 sResolutionToggleDelay;

s32 OptionsMenu_ToggleResolution(void) {
    if (sResolutionToggleDelay == 0.0f) {
        func_0042452C();
        sResolutionToggleDelay = D_800C132C;
        gEngineApp_m_Settings.unk180 = D_800C203B;
    }
    return 0;
}
