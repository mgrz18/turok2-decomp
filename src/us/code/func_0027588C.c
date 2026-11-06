#include "common.h"

void CAudioWorld__StopSound(void *, s32);              /* extern */
extern u8 gAudioWorld[];

void Sound__StopSoundId(s32 arg0) {
    CAudioWorld__StopSound(gAudioWorld, arg0);
}
