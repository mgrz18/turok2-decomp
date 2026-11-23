#include "common.h"

s32 SndSeq_Unduck(s32);                         /* extern */

void CAudioWorld__UnduckMusic(sptr arg0) {
    SndSeq_Unduck(arg0 + 0x1D64);
}
