#include "common.h"
#include "m2c_macros.h"

s32 Video_StartResolutionChange_2(void);

s32 PauseMenu_ToggleResolution_2(void) {
    Video_StartResolutionChange_2();
    return 0;
}
