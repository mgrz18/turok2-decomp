#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"

s32 Cinema_RequestForObject(sptr, sptr);


void Event_PlayCinema(sptr arg0, sptr arg1, sptr arg2) {
    if (gScene.m_Mode == 4) {
        Cinema_RequestForObject(arg0, arg2);
    }
}
