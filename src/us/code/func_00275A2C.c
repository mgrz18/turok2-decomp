#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__StopMusic(s32 *);                   /* extern */

void Music_Stop(void) {
    CAudioWorld__StopMusic(&gAudioWorld);
}
