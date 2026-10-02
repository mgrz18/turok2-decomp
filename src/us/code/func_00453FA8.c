#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8[];

s32 func_00453FA8(s32 arg0, s32 arg1) {
    D_8011AAD8[0] ^= arg1;
    return 0;
}
