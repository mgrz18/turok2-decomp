#include "common.h"

s32 CAudioWorld__Initialize(s32 *);                   /* extern */
extern s32 gAudioWorld;

void Sound__Initialize(void) {
    CAudioWorld__Initialize(&gAudioWorld);
}
