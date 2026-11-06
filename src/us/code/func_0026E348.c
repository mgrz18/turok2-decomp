#include "common.h"
#include "m2c_macros.h"

s32 Cinema_RequestForObject(s32, s32);

extern s32 gScene;

void Event_PlayCinema(s32 arg0, s32 arg1, s32 arg2) {
    if (gScene == 4) {
        Cinema_RequestForObject(arg0, arg2);
    }
}
