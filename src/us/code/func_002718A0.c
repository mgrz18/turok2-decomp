#include "common.h"

s32 SndSeq_FadeOut(s32);                         /* extern */

void CAudioWorld__FadeOutMusic(s32 arg0) {
    SndSeq_FadeOut(arg0 + 0x1D64);
}
