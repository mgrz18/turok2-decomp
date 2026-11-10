#include "common.h"

void SndSeq_Stop(s32);                         /* extern */

void CAudioWorld__StopMusic(sptr arg0) {
    SndSeq_Stop(arg0 + 0x1D64);
}
