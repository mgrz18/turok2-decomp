#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__StopSpeech(s32 *);                   /* extern */

void Sound__StopSpeech(void) {
    CAudioWorld__StopSpeech(&gAudioWorld);
}
