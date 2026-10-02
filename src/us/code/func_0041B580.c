#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

s32 func_0041B580(void) {
    D_8011AAD8 ^= 0x80;
    return 0;
}
