#include "common.h"

s32 CAudioWorld__SetListener(s32 *, s32);              /* extern */
extern s32 gAudioWorld;

void Sound__SetListener(s32 arg0) {
    CAudioWorld__SetListener(&gAudioWorld, arg0);
}
