#include "common.h"
#include "m2c_macros.h"

extern s32 gAudioWorld_seq_track;

s32 Music_GetTrack(void) {
    return gAudioWorld_seq_track;
}
