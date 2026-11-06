#include "common.h"
#include "m2c_macros.h"

extern s32 gMenuSoundsEnabled;

void Controller_DisableMenuSounds(void) {
    gMenuSoundsEnabled = 0;
}
