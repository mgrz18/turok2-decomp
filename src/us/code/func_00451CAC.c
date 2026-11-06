#include "common.h"
#include "structs/game.h"

/* The multiplayer settings, D_8011AAD8 (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 func_00451CAC(s32 arg0, s32 arg1) {
    GameSettings *settings = &D_8011AAD8;

    settings->dmTextureSet = Menu_StepValue(arg1, settings->dmTextureSet, 1, 0, 8, 1);
    return 0;
}
