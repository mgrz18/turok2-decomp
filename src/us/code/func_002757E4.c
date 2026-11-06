#include "common.h"

s32 CAudioWorld__CountHandleVoices(s32 *, s32);              /* extern */
extern s32 D_800F2A00;

void Sound__IsPlaying(s32 arg0) {
    CAudioWorld__CountHandleVoices(&D_800F2A00, arg0);
}
