#include "common.h"
#include "m2c_macros.h"

s32 Cinema_RequestForObject(s32, s32);

extern s32 D_800F7078;

void Event_PlayCinema(s32 arg0, s32 arg1, s32 arg2) {
    if (D_800F7078 == 4) {
        Cinema_RequestForObject(arg0, arg2);
    }
}
