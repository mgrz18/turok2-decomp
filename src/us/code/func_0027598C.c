#include "common.h"

void CAudioWorld__SetForce2D(void *, s32);              /* extern */
extern u8 gAudioWorld[];

void Sound__SetForce2D(s32 arg0) {
    CAudioWorld__SetForce2D(gAudioWorld, arg0);
}
