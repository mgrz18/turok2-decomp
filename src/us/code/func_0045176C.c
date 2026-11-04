#include "common.h"
#include "structs/game.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

/* The multiplayer settings, D_8011AAD8 (0x184 bytes, see func_0042F514). */


s32 func_0045176C(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &D_8011AAD8;
    var_s0 = &D_8011AAD8;
    D_8011AAD8.tagUnk29 = func_0041648C(arg1, (s32) D_8011AAD8.tagUnk29, 1, 0, 1, 1);
    return 0;
}
