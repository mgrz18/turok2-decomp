#include "common.h"

s32 CAudioWorld__StopSpeech(s32 *);                   /* extern */
extern s32 gAudioWorld;

void Sound__StopSpeech(void) {
    CAudioWorld__StopSpeech(&gAudioWorld);
}
