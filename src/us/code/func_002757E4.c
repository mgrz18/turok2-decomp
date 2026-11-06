#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__CountHandleVoices(s32 *, s32);              /* extern */

void Sound__IsPlaying(s32 arg0) {
    CAudioWorld__CountHandleVoices(&gAudioWorld, arg0);
}
