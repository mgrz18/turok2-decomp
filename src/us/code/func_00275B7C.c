#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__Lock(s32 *);                   /* extern */

void Sound__Lock(void) {
    CAudioWorld__Lock(&gAudioWorld);
}
