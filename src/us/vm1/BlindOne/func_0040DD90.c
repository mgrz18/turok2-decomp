#include "common.h"
#include "m2c_macros.h"

void Sound__Play2D(s32 sound);

void BlindOne_BossDeathEnter(void) {
    Sound__Play2D(0x284E);
}
