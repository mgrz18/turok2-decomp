#include "common.h"

s32 Cinema_RequestForObject(s32, s32);                /* extern */

void InteractiveAnim_EventPlayCinemaRemote(s32 arg0) {
    Cinema_RequestForObject(0, arg0);
}
