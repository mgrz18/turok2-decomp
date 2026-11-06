#include "common.h"
#include "globals/game.h"
#include "globals/audio.h"

s32 CAudioWorld__PlaySpeech(s32 *, s32);              /* extern */

void Sound__PlaySpeech(s32 arg0) {
    if (gEngineApp_m_Settings.unk1C8 == 0) {
        CAudioWorld__PlaySpeech(&gAudioWorld, arg0);
    }
}
