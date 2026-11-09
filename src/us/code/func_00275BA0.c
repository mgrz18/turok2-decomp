#include "common.h"
#include "globals/audio.h"

extern void CAudioWorld__Unlock(void *);

#ifdef TARGET_PORT
int T2Port_AudioReady(void);
#endif
void Sound__Unlock(void) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__Unlock(&gAudioWorld);
}
