#include "common.h"
#include "globals/game.h"
#include "globals/audio.h"

s32 CAudioWorld__PlaySpeech(s32 *, s32);              /* extern */

#ifdef TARGET_PORT
int T2Port_AudioReady(void);
#endif
void Sound__PlaySpeech(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    if (gEngineApp_m_Settings.unk1C8 == 0) {
        CAudioWorld__PlaySpeech(&gAudioWorld, arg0);
    }
}
