#include "common.h"

s32 Video_StartResolutionChange_2();                            /* extern */

s32 PauseMenu_ToggleResolution_2(void) {
    Video_StartResolutionChange_2();
    return 0;
}
