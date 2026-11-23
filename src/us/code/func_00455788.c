#include "common.h"
#include "globals/game.h"
#include "globals/menu.h"

s32 Video_StartResolutionChange_2();                            /* extern */
extern u8 D_800C203B;

s32 OptionsMenu_ToggleResolution_2(void) {
    if (sResolutionToggleDelay == 0.0f) {
        Video_StartResolutionChange_2();
        sResolutionToggleDelay = 18.0f;
        gEngineApp_m_Settings.unk180 = D_800C203B;
    }
    return 0;
}
