#include "common.h"
#include "structs/sound.h"

void CAudioWorld__SetListener(SndMgr *arg0, s32 arg1) {
    arg0->listener = arg1;
}
