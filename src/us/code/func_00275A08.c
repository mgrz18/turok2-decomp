#include "common.h"

s32 CAudioWorld__FadeOutMusic(s32 *);                   /* extern */
extern s32 gAudioWorld;

void Music_FadeOut(void) {
    CAudioWorld__FadeOutMusic(&gAudioWorld);
}
