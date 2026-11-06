#include "common.h"

s32 Sound__DisableAmbientTrack();                            /* extern */
s32 Sound__EnableAmbientTrack();                            /* extern */
extern s32 gAudioWorld_track;

void Sound__SetAmbientTrack(s32 arg0) {
    gAudioWorld_track = arg0;
    if (arg0 != 0) {
        Sound__EnableAmbientTrack();
        return;
    }
    Sound__DisableAmbientTrack();
}
