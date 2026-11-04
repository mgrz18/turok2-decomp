#include "common.h"
#include "structs/sound.h"

u32 func_00276868(SoundEffectInfo *arg0) {
    return (u16) arg0->flags >> 0xE;
}
