#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__SetAmbientTrackEnabled(s32 *, s32);          /* extern */

void Sound__EnableAmbientTrack(void) {
    CAudioWorld__SetAmbientTrackEnabled(&gAudioWorld, 1);
}
