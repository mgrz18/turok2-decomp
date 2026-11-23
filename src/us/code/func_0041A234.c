#include "common.h"
#include "m2c_macros.h"

s32 Video_StartResolutionChange(void);

s32 PauseMenu_ToggleResolution(void) {
    Video_StartResolutionChange();
    return 0;
}
