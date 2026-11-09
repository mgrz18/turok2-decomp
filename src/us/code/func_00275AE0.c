#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__SetListener(s32 *, sptr);              /* extern */

void Sound__SetListener(sptr arg0) {
    CAudioWorld__SetListener(&gAudioWorld, arg0);
}
