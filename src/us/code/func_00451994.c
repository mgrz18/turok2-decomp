#include "common.h"
#include "structs/game.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00451994(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &D_8011AAD8;
    var_s0 = &D_8011AAD8;
    D_8011AAD8.dmTimeLimit = func_0041648C(arg1, (s32) D_8011AAD8.dmTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}
