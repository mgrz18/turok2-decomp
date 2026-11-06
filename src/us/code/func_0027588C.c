#include "common.h"

void CAudioWorld__StopSound(void *, s32);              /* extern */
extern u8 D_800F2A00[];

void Sound__StopSoundId(s32 arg0) {
    CAudioWorld__StopSound(D_800F2A00, arg0);
}
