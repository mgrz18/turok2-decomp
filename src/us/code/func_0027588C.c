#include "common.h"
#include "globals/audio.h"

void CAudioWorld__StopSound(void *, s32);              /* extern */

void Sound__StopSoundId(s32 arg0) {
    CAudioWorld__StopSound(&gAudioWorld, arg0);
}
