#include "common.h"
#include "structs/sound.h"

s32 CISet__GetBlock(s32, s16);                        /* extern */

void CAudioWorld__GetVariantCue(SndMgr *arg0, s16 arg1) {
    CISet__GetBlock(CISet__GetBlock(arg0->variants, arg1), 0);
}
