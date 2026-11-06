#include "common.h"
#include "structs/game.h"

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 func_004513B4(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &D_8011AAD8;
    var_s0 = &D_8011AAD8;
    D_8011AAD8.tagTimeLimit = Menu_StepValue(arg1, (s32) D_8011AAD8.tagTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}
