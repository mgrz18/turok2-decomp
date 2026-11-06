#include "common.h"

s32 CAudioWorld__StopHandle(s32 *, s32);              /* extern */
extern s32 gAudioWorld;

void Sound__Stop(s32 arg0) {
    CAudioWorld__StopHandle(&gAudioWorld, arg0);
}
