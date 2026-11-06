#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

s32 CheatMenu_ToggleBlackout(void) {
    D_8011AAD8 ^= 0x4000;
    return 0;
}
