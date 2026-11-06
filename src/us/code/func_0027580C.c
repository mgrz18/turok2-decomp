#include "common.h"
#include "globals/game.h"

s32 CAudioWorld__PlaySpeech(s32 *, s32);              /* extern */
extern s32 gAudioWorld;

void Sound__PlaySpeech(s32 arg0) {
    if (gEngineApp_m_Settings.unk1C8 == 0) {
        CAudioWorld__PlaySpeech(&gAudioWorld, arg0);
    }
}
