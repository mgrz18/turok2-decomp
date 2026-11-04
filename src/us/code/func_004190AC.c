#include "common.h"
#include "structs/game.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_004190AC(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &D_8011AAD8;
    var_s0 = &D_8011AAD8;
    D_8011AAD8.teamArena = func_0041648C(arg1, (s32) D_8011AAD8.teamArena, 1, 0, 0xD, 1);
    return 0;
}
