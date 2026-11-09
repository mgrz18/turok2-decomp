#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__SetFxMix(s32 *, f32);              /* extern */

#ifdef TARGET_PORT
int T2Port_AudioReady(void);
#endif
void Sound__SetReverb(f32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__SetFxMix(&gAudioWorld, arg0);
}
