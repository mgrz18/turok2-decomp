#include "common.h"
#include "m2c_macros.h"

s32 Cinema_RequestById_2(s32);

void BlindOne_PhaseStartCinemaInit_2(void) {
    Cinema_RequestById_2(0x15E);
}
