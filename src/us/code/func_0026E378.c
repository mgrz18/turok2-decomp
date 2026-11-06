#include "common.h"

void Cinema_RequestForObject(s32, s32);                /* extern */
extern s32 gScene;

void Event_PlayCinemaRemote(s32 arg0) {
    if (gScene == 4) {
        Cinema_RequestForObject(0, arg0);
    }
}
