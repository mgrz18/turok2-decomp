#include "common.h"
#include "globals/scene.h"

void Cinema_RequestForObject(sptr, s32);                /* extern */

void Event_PlayCinemaRemote(s32 arg0) {
    if (gScene.m_Mode == 4) {
        Cinema_RequestForObject(0, arg0);
    }
}
