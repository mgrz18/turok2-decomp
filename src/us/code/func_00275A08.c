#include "common.h"
#include "globals/audio.h"

void CAudioWorld__FadeOutMusic(CAudioWorld *);              /* extern */

void Music_FadeOut(void) {
    CAudioWorld__FadeOutMusic(&gAudioWorld);
}
