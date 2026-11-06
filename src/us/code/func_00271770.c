#include "common.h"
#include "structs/sound.h"

void CAudioWorld__SetMusicVolume(SndMgr *arg0, f32 arg1) {
    arg0->musicVolume = arg1;
}
