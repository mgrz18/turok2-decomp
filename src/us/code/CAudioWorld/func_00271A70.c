#include "common.h"

s32 SndSeq_Duck(s32);                         /* extern */

void CAudioWorld__DuckMusic(sptr arg0) {
    SndSeq_Duck(arg0 + 0x1D64);
}
