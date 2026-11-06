#include "common.h"

s32 CAudioWorld__DetachAllVoices(s32 *);                   /* extern */
extern s32 gAudioWorld;

void Sound__DetachAll(void) {
    CAudioWorld__DetachAllVoices(&gAudioWorld);
}
