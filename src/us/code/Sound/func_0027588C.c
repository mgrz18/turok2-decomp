#include "common.h"
#include "globals/audio.h"

void CAudioWorld__StopSound(void *, s32);              /* extern */

#ifdef TARGET_PORT
int T2Port_AudioReady(void);
#endif
void Sound__StopSoundId(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__StopSound(&gAudioWorld, arg0);
}
