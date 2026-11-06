#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

s32 CheatMenu_ToggleAllMap_2(void) {
    D_8011AAD8 ^= 0x2000;
    return 0;
}
