#include "common.h"
#include "m2c_macros.h"

extern s32 D_800B6CF8;

void Controller_DisableMenuSounds(void) {
    D_800B6CF8 = 0;
}
