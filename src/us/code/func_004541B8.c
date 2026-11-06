#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

s32 CheatMenu_ToggleInfiniteLives_2(void) {
    D_8011AAD8 ^= 2;
    return 0;
}
