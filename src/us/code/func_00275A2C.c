#include "common.h"

s32 CAudioWorld__StopMusic(s32 *);                   /* extern */
extern s32 gAudioWorld;

void Music_Stop(void) {
    CAudioWorld__StopMusic(&gAudioWorld);
}
