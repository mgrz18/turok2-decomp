#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"
#include "globals/menu.h"

s32 Video_StartResolutionChange(void);

extern u8 D_800C203B;

s32 OptionsMenu_ToggleResolution(void) {
    if (sResolutionToggleDelay == 0.0f) {
        Video_StartResolutionChange();
        sResolutionToggleDelay = 18.0f;
        gEngineApp_m_Settings.unk180 = D_800C203B;
    }
    return 0;
}
