#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__SetMuted(s32 *, s32);              /* extern */

#ifdef TARGET_PORT
int T2Port_AudioReady(void);
#endif
void Sound__SetMuted(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__SetMuted(&gAudioWorld, arg0);
}
