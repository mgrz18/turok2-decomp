#include "common.h"

s32 CAudioWorld__StopSoundsByOwner(s32 *, s32);              /* extern */
extern s32 D_800F2A00;

void Sound__StopOwner(s32 arg0) {
    CAudioWorld__StopSoundsByOwner(&D_800F2A00, arg0);
}
