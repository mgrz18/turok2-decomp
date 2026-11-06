#include "common.h"

s32 CAudioWorld__PlaySpeech(s32 *, s32);              /* extern */
extern s32 gAudioWorld;
extern s32 D_8011ACA0;

void Sound__PlaySpeech(s32 arg0) {
    if (D_8011ACA0 == 0) {
        CAudioWorld__PlaySpeech(&gAudioWorld, arg0);
    }
}
