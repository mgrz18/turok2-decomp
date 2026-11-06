#include "common.h"

s32 CAudioWorld__StopSoundsByFlags(s32 *, s32);              /* extern */
extern s32 D_800F2A00;

void Sound__StopFlags(s32 arg0) {
    CAudioWorld__StopSoundsByFlags(&D_800F2A00, arg0);
}
