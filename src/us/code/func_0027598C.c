#include "common.h"
#include "globals/audio.h"

void CAudioWorld__SetForce2D(void *, s32);              /* extern */

void Sound__SetForce2D(s32 arg0) {
    CAudioWorld__SetForce2D(&gAudioWorld, arg0);
}
