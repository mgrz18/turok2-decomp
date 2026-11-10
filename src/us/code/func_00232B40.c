#include "common.h"
#include "protos/game.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x1B8];
    /* 0x1B8 */ s32 loop;               /* looping sound handle */
} SoundObject;

s32 LoopSounds_Stop(sptr, sptr);                    /* extern */

#ifdef TARGET_PORT
#include "structs/particle.h"
/* the host CParticle's m_pLoopSound, pointer-wide */
void CParticle__StopLoopSound(SoundObject *arg0) {
    CParticle *p = (CParticle *) arg0;
    if (p->m_pLoopSound != NULL) {
        LoopSounds_Stop(LoopSounds_Get(), (sptr) p->m_pLoopSound);
        p->m_pLoopSound = NULL;
    }
}
#else
void CParticle__StopLoopSound(SoundObject *arg0) {
    if (arg0->loop != 0) {
        LoopSounds_Stop(LoopSounds_Get(), arg0->loop);
        arg0->loop = 0;
    }
}
#endif
