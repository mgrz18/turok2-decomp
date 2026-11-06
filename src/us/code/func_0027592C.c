#include "common.h"

s32 CAudioWorld__Update(s32 *);                   /* extern */
extern s32 gAudioWorld;

void Sound__Update(void) {
    CAudioWorld__Update(&gAudioWorld);
}
