#include "common.h"

s32 CAudioWorld__StopAllSounds(s32 *, s32);              /* extern */
extern s32 D_800F2A00;

void Sound__StopAllExcept(s32 arg0) {
    CAudioWorld__StopAllSounds(&D_800F2A00, arg0);
}
