#include "common.h"
#include "m2c_macros.h"

s32 CPlayer__StartDeathCinema(s32, s32, s32);

void PlayerDeath32_Enter(s32 arg0, s32 arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0x32);
}
