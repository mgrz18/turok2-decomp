#include "common.h"
#include "m2c_macros.h"

extern s32 sDamagedPakInGame;
extern s32 sDamagedPakDone;

void DamagedPakMenu_InitInGame(void) {
    sDamagedPakDone = 0;
    sDamagedPakInGame = 1;
}
