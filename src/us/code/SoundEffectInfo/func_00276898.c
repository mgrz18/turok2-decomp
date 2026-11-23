#include "common.h"
#include "structs/sound.h"

s32 SoundEffectInfo_GetFlags(SoundEffectInfo *arg0) {
    return arg0->flags & 0x3FFF;
}
