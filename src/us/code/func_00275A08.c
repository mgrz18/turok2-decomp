#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__FadeOutMusic(s32 *);                   /* extern */

void Music_FadeOut(void) {
    CAudioWorld__FadeOutMusic(&gAudioWorld);
}
