#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

s32 CheatMenu_ToggleAllWeapons(void) {
    D_8011AAD8 ^= 4;
    return 0;
}
