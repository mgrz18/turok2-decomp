#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__StopSoundsByOwner(s32 *, s32);              /* extern */

void Sound__StopOwner(s32 arg0) {
    CAudioWorld__StopSoundsByOwner(&gAudioWorld, arg0);
}
