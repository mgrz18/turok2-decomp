#include "common.h"
#include "structs/game.h"

/* The multiplayer settings, D_8011AAD8 (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 func_00451E54(s32 arg0, s32 arg1) {
    GameSettings *settings = &D_8011AAD8;

    settings->teamTimeLimit = Menu_StepValue(arg1, settings->teamTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}
