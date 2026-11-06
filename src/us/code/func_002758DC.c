#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__StopSoundsByFlags(s32 *, s32);              /* extern */

void Sound__StopFlags(s32 arg0) {
    CAudioWorld__StopSoundsByFlags(&gAudioWorld, arg0);
}
