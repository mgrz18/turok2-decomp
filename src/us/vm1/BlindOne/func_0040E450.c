#include "common.h"
#include "m2c_macros.h"

s32 Cinema_RequestById(s32);

void BlindOne_PhaseStartCinemaInit(void) {
    Cinema_RequestById(0x15E);
}
