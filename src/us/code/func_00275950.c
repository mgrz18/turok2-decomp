#include "common.h"
#include "globals/audio.h"

s32 Sound__DisableAmbientTrack();                            /* extern */
s32 Sound__EnableAmbientTrack();                            /* extern */

#ifdef TARGET_PORT
int T2Port_AudioReady(void);
#endif
void Sound__SetAmbientTrack(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    gAudioWorld.track = arg0;
    if (arg0 != 0) {
        Sound__EnableAmbientTrack();
        return;
    }
    Sound__DisableAmbientTrack();
}
