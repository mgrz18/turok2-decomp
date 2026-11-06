#include "common.h"

s32 CAudioWorld__SetAmbientTrackEnabled(s32 *, s32);          /* extern */
extern s32 gAudioWorld;

void Sound__EnableAmbientTrack(void) {
    CAudioWorld__SetAmbientTrackEnabled(&gAudioWorld, 1);
}
