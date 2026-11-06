#include "common.h"

s32 CAudioWorld__PlaySpeech(s32 *, s32);              /* extern */
extern s32 gAudioWorld;
extern s32 gEngineApp_m_Settings_unk1C8;

void Sound__PlaySpeech(s32 arg0) {
    if (gEngineApp_m_Settings_unk1C8 == 0) {
        CAudioWorld__PlaySpeech(&gAudioWorld, arg0);
    }
}
