#include "common.h"
#include "m2c_macros.h"

s32 Sound__Play2D(s32);

void BlindOne_BossDeathEnter(void) {
    Sound__Play2D(0x284E);
}
