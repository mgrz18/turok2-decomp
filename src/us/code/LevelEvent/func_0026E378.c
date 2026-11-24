#include "common.h"
#include "globals/scene.h"
#include "structs/event.h"

void Cinema_RequestForObject(void *owner, LevelEvent *obj);

void Event_PlayCinemaRemote(sptr arg0) {
    if (gScene.m_Mode == 4) {
        Cinema_RequestForObject(NULL, (LevelEvent *) arg0);
    }
}
