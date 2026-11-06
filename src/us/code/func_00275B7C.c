#include "common.h"

s32 CAudioWorld__Lock(s32 *);                   /* extern */
extern s32 gAudioWorld;

void Sound__Lock(void) {
    CAudioWorld__Lock(&gAudioWorld);
}
