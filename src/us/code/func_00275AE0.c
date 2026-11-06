#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__SetListener(s32 *, s32);              /* extern */

void Sound__SetListener(s32 arg0) {
    CAudioWorld__SetListener(&gAudioWorld, arg0);
}
