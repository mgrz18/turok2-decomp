#include "common.h"
#include "structs/sound.h"

void CAudioWorld__SetSequenceVolume(SndMgr *arg0, u32 arg1) {
    arg0->volume2BA4 = arg1;
}
