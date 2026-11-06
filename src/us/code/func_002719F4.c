#include "common.h"

void SndSeq_Stop(s32);                         /* extern */

void CAudioWorld__StopMusic(s32 arg0) {
    SndSeq_Stop(arg0 + 0x1D64);
}
