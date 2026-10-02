#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

s32 func_004543D8(void) {
    D_8011AAD8 ^= 0x20;
    return 0;
}
