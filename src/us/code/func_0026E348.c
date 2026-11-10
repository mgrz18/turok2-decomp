#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"

s32 Cinema_RequestForObject(sptr, s32);


void Event_PlayCinema(s32 arg0, s32 arg1, s32 arg2) {
    if (gScene.m_Mode == 4) {
        Cinema_RequestForObject(arg0, arg2);
    }
}
