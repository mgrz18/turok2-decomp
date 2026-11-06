#include "common.h"

s32 CAudioWorld__SetFxMix(s32 *, f32);              /* extern */
extern s32 D_800F2A00;

void Sound__SetReverb(f32 arg0) {
    CAudioWorld__SetFxMix(&D_800F2A00, arg0);
}
