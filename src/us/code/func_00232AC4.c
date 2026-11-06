#include "common.h"
#include "structs/sound.h"

/* An object with a looping sound attached. */
typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ f32 pos[3];
    /* 0x010 */ u8 pad10[0x1B8 - 0x10];
    /* 0x1B8 */ SndSource *loop;
} SoundObject;

s32 LoopSounds_Start(s32, s32, s32, s32, s32);
s32 CParticle__StopLoopSound(SoundObject *);                      /* extern */
s32 LoopSounds_Get();                                /* extern */

/* Starts (or restarts) the looping sound on the object. */
void CParticle__StartLoopSound(SoundObject *arg0, s32 arg1) {
    s32 var_s1;
    f32 *temp_a2;
    SndSource *temp_v1;
    SoundObject *var_s0;

    var_s0 = arg0;
    temp_v1 = var_s0->loop;
    var_s1 = arg1;
    if (temp_v1 != NULL) {
        if ((temp_v1->sound.w != var_s1) || (temp_v1->handle == -1)) {
            CParticle__StopLoopSound(var_s0);
            goto block_4;
        }
    } else {
block_4:
        temp_a2 = var_s0->pos;
        var_s0->loop = (SndSource *) LoopSounds_Start(LoopSounds_Get(), var_s1, (s32) temp_a2, (s32) temp_a2, -1);
    }
}
