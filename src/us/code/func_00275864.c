#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__StopHandle(s32 *, s32);              /* extern */

void Sound__Stop(s32 arg0) {
    CAudioWorld__StopHandle(&gAudioWorld, arg0);
}
