#include "common.h"
#include "structs/game.h"

/* The multiplayer settings, D_8011AAD8 (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 OptionsMenu_StepBrightness_2(s32 arg0, s32 arg1) {
    GameSettings *settings = &D_8011AAD8;

    settings->unkC = Menu_StepValue(arg1, settings->unkC, 0x10, 0, 0x80, 0);
    return 0;
}
