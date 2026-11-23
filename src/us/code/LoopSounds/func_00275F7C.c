#include "common.h"
#include "globals/audio.h"
#include "protos/game.h"

LoopSounds *LoopSounds_Get(void) {
    return &gLoopSounds;
}
