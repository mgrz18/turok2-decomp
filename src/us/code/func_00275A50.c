#include "common.h"

s32 CAudioWorld__DuckMusic(s32 *);                   /* extern */
extern s32 gAudioWorld;

void Music_DuckVolume(void) {
    CAudioWorld__DuckMusic(&gAudioWorld);
}
