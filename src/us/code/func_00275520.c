#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__Initialize(s32 *);                   /* extern */

void Sound__Initialize(void) {
    CAudioWorld__Initialize(&gAudioWorld);
}
