#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

s32 func_00454790(void) {
    D_8011AAD8 ^= 0x1000;
    return 0;
}
