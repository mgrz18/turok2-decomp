#include "common.h"
#include "globals/audio.h"

extern void CAudioWorld__Unlock(void *);

void Sound__Unlock(void) {
    CAudioWorld__Unlock(&gAudioWorld);
}
