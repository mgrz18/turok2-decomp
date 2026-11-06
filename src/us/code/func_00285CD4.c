#include "common.h"
#include "m2c_macros.h"

extern s32 D_800B6CF8;

void Controller_EnableMenuSounds(void) {
    D_800B6CF8 = 1;
}
