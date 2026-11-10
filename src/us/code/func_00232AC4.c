#include "common.h"
#include "structs/sound.h"
#include "structs/particle.h"
#include "protos/game.h"

sptr LoopSounds_Start(sptr, s32, s32, s32, sptr);
s32 CParticle__StopLoopSound(CParticle *);                      /* extern */

/* Starts (or restarts) the looping sound on the object. */
void CParticle__StartLoopSound(CParticle *arg0, s32 arg1) {
    s32 var_s1;
    CVector3 *temp_a2;
    SndSource *temp_v1;
    CParticle *var_s0;

    var_s0 = arg0;
    temp_v1 = var_s0->m_pLoopSound;
    var_s1 = arg1;
    if (temp_v1 != NULL) {
        if ((temp_v1->sound.w != var_s1) || (temp_v1->handle == -1)) {
            CParticle__StopLoopSound(var_s0);
            goto block_4;
        }
    } else {
block_4:
        temp_a2 = &var_s0->header.m_vPos;
        var_s0->m_pLoopSound = (SndSource *) LoopSounds_Start(LoopSounds_Get(), var_s1, (sptr) temp_a2, (sptr) temp_a2, -1);
    }
}
