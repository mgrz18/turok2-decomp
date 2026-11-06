#include "common.h"

extern char gAudioWorld[];
extern void CAudioWorld__Unlock(void *);

void Sound__Unlock(void) {
    CAudioWorld__Unlock(gAudioWorld);
}
