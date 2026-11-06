#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__StopAllSounds(s32 *, s32);              /* extern */

void Sound__StopAllExcept(s32 arg0) {
    CAudioWorld__StopAllSounds(&gAudioWorld, arg0);
}
