#include "common.h"

s32 CAudioWorld__SetFxMix(s32 *, f32);              /* extern */
extern s32 gAudioWorld;

void Sound__SetReverb(f32 arg0) {
    CAudioWorld__SetFxMix(&gAudioWorld, arg0);
}
