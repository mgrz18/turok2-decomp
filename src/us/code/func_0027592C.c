#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__Update(s32 *);                   /* extern */

void Sound__Update(void) {
    CAudioWorld__Update(&gAudioWorld);
}
