#include "common.h"
#include "globals/audio.h"

void CAudioWorld__UnduckMusic(void *);                   /* extern */

void Music_RestoreVolume(void) {
    CAudioWorld__UnduckMusic(&gAudioWorld);
}
