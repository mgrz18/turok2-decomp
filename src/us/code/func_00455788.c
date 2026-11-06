#include "common.h"

s32 func_0045D44C();                            /* extern */
extern f32 D_800C132C;
extern u8 D_800C203B;
extern u8 gEngineApp_m_Settings_unk180;
extern f32 sResolutionToggleDelay;

s32 OptionsMenu_ToggleResolution_2(void) {
    if (sResolutionToggleDelay == 0.0f) {
        func_0045D44C();
        sResolutionToggleDelay = D_800C132C;
        gEngineApp_m_Settings_unk180 = D_800C203B;
    }
    return 0;
}
