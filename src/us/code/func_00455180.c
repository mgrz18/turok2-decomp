#include "common.h"
#include "m2c_macros.h"

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

extern u8 D_8011AAEF[];

s32 OptionsMenu_StepBloodColor_2(s32 arg0, s32 arg1) {
    D_8011AAEF[0] = Menu_StepValue(arg1, (s32) D_8011AAEF[0], 1, 0, 2, 1);
    return 0;
}
