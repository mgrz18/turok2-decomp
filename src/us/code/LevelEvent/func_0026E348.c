#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"
#include "structs/event.h"

void Cinema_RequestForObject(void *owner, LevelEvent *obj);


void Event_PlayCinema(sptr arg0, sptr arg1, sptr arg2) {
    if (gScene.m_Mode == 4) {
        Cinema_RequestForObject((void *) arg0, (LevelEvent *) arg2);
    }
}
