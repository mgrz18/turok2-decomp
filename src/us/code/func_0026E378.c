#include "common.h"
#include "globals/scene.h"

void Cinema_RequestForObject(sptr, sptr);                /* extern */

void Event_PlayCinemaRemote(sptr arg0) {
    if (gScene.m_Mode == 4) {
        Cinema_RequestForObject(0, arg0);
    }
}
