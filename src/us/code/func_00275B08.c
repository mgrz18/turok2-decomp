#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__SetMuted(s32 *, s32);              /* extern */

void Sound__SetMuted(s32 arg0) {
    CAudioWorld__SetMuted(&gAudioWorld, arg0);
}
