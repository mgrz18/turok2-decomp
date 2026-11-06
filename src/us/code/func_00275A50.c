#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__DuckMusic(s32 *);                   /* extern */

void Music_DuckVolume(void) {
    CAudioWorld__DuckMusic(&gAudioWorld);
}
