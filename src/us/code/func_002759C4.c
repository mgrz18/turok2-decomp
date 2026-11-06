#include "common.h"
#include "globals/audio.h"

s32 Music_FadeOut();                            /* extern */
extern s32 gEngineApp_m_Settings_unk1C8[]; /* local type: the field's real type (or the member path) changes the ROM's code here */

void Music_SetTrack(s32 arg0) {
    if ((gEngineApp_m_Settings_unk1C8[0] == 0) && (gAudioWorld.seq.track != arg0)) {
        gAudioWorld.seq.track = arg0;
        if (arg0 == 0) {
            Music_FadeOut();
        }
    }
}
