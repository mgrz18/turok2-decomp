#include "common.h"
#include "m2c_macros.h"

s32 Cinema_RequestById(s32);

void BlindOne_PhaseStartCinemaInit_2(void) {
    Cinema_RequestById(0x15E);
}
