#include "common.h"
#include "globals/audio.h"
#include "globals/game.h"

s32 Music_FadeOut();                            /* extern */

void Music_SetTrack(s32 arg0) {
    if ((gEngineApp_m_Settings_unk1C8[0] == 0) && (gAudioWorld.seq.track != arg0)) {
        gAudioWorld.seq.track = arg0;
        if (arg0 == 0) {
            Music_FadeOut();
        }
    }
}
