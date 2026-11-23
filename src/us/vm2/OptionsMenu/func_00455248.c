#include "common.h"
#include "globals/game.h"

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

s32 OptionsMenu_StepTwoPlayerLayout_2(s32 arg0, s32 arg1) {
    u8 *value = &gEngineApp_m_Settings_unk1B; /* through a pointer: the ROM builds the address once */

    *value = Menu_StepValue_2(arg1, *value, 1, 0, 1, 1);
    return 0;
}
