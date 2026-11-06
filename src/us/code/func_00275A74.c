#include "common.h"

void CAudioWorld__UnduckMusic(void *);                   /* extern */
extern u8 gAudioWorld[];

void Music_RestoreVolume(void) {
    CAudioWorld__UnduckMusic(gAudioWorld);
}
