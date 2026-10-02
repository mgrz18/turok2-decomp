#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

s32 func_00454130(void) {
    D_8011AAD8 ^= 1;
    return 0;
}
