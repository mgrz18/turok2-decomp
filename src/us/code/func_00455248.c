#include "common.h"
#include "m2c_macros.h"

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

extern u8 D_8011AAF3[];

s32 OptionsMenu_StepTwoPlayerLayout_2(s32 arg0, s32 arg1) {
    D_8011AAF3[0] = Menu_StepValue_2(arg1, (s32) D_8011AAF3[0], 1, 0, 1, 1);
    return 0;
}
