#include "common.h"

void Cinema_RequestForObject(s32, s32);                /* extern */
extern s32 D_800F7078;

void Event_PlayCinemaRemote(s32 arg0) {
    if (D_800F7078 == 4) {
        Cinema_RequestForObject(0, arg0);
    }
}
