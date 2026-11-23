#include "common.h"
#include "structs/sound.h"

u32 SoundEffectInfo_GetPan(SoundEffectInfo *arg0) {
    return (u16) arg0->flags >> 0xE;
}
