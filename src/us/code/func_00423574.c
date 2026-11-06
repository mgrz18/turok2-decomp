#include "common.h"
#include "m2c_macros.h"

extern s32 sDamagedPakInGame;
extern s32 sDamagedPakDone;

void DamagedPakMenu_InitAtBoot(void) {
    sDamagedPakDone = 0;
    sDamagedPakInGame = 0;
}
