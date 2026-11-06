#include "common.h"

s32 CAudioWorld__SetAmbientTrackEnabled(s32 *, s32);          /* extern */
extern s32 D_800F2A00;

void Sound__DisableAmbientTrack(void) {
    CAudioWorld__SetAmbientTrackEnabled(&D_800F2A00, 0);
}
