#include "common.h"
#include "structs/game.h"

/* The multiplayer settings, D_8011AAD8 (GameSettings in func_00450F4C; 0x184 bytes, see func_0042F514). */

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00451E54(s32 arg0, s32 arg1) {
    GameSettings *settings = &D_8011AAD8;

    settings->teamTimeLimit = func_0041648C(arg1, settings->teamTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}
