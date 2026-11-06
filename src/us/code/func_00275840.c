#include "common.h"

s32 CAudioWorld__StopSpeech(s32 *);                   /* extern */
extern s32 D_800F2A00;

void Sound__StopSpeech(void) {
    CAudioWorld__StopSpeech(&D_800F2A00);
}
