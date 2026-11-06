#include "common.h"

s32 SpeechPlayer_Stop(s32);                         /* extern */

void CAudioWorld__StopSpeech(s32 arg0) {
    SpeechPlayer_Stop(arg0 + 0x2BC0);
}
