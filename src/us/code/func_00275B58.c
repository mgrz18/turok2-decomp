#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__DetachAllVoices(s32 *);                   /* extern */

void Sound__DetachAll(void) {
    CAudioWorld__DetachAllVoices(&gAudioWorld);
}
