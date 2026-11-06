#include "common.h"

s32 CAudioWorld__PlaySpeech(s32 *, s32);              /* extern */
extern s32 D_800F2A00;
extern s32 D_8011ACA0;

void Sound__PlaySpeech(s32 arg0) {
    if (D_8011ACA0 == 0) {
        CAudioWorld__PlaySpeech(&D_800F2A00, arg0);
    }
}
