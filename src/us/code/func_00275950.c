#include "common.h"
#include "globals/audio.h"

s32 Sound__DisableAmbientTrack();                            /* extern */
s32 Sound__EnableAmbientTrack();                            /* extern */

void Sound__SetAmbientTrack(s32 arg0) {
    gAudioWorld.track = arg0;
    if (arg0 != 0) {
        Sound__EnableAmbientTrack();
        return;
    }
    Sound__DisableAmbientTrack();
}
