#include "common.h"
#include "protos/game.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x1B8];
    /* 0x1B8 */ s32 loop;               /* looping sound handle */
} SoundObject;

s32 LoopSounds_Stop(s32, s32);                    /* extern */

void CParticle__StopLoopSound(SoundObject *arg0) {
    if (arg0->loop != 0) {
        LoopSounds_Stop(LoopSounds_Get(), arg0->loop);
        arg0->loop = 0;
    }
}
