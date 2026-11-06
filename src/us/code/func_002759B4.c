#include "common.h"
#include "m2c_macros.h"
#include "globals/audio.h"


s32 Music_GetTrack(void) {
    return gAudioWorld.seq.track;
}
