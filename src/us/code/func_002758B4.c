#include "common.h"

s32 CAudioWorld__StopSoundsByOwner(s32 *, s32);              /* extern */
extern s32 gAudioWorld;

void Sound__StopOwner(s32 arg0) {
    CAudioWorld__StopSoundsByOwner(&gAudioWorld, arg0);
}
