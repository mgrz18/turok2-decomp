#include "common.h"
#include "structs/game_object.h"
#include "structs/particle.h"
#include "globals/scene.h"
#ifdef TARGET_PORT
#include "structs/player.h"
#endif

s32 CParticleSystem__Free(CParticleSystem *, void *);           /* extern */
s32 CParticle__StopLoopSound(void *);                      /* extern */
s16 *CPlayer__GetFiringAmmo(sptr, s32);                   /* extern */

/* The parent actor block reached through actor->m_AI.m_pParent. */
typedef struct {
    /* 0x000 */ u8 pad0[0x934];
    /* 0x934 */ s16 flags934[1];
} Parent0023181C;

void func_0023181C(CParticle *arg0) {
    sptr temp_s0;
    u16 temp_v1;
    u8 temp_a0;
    CParticle *temp_s0_2;

    if (arg0->header.m_nObjType == 0x68) {
        temp_s0 = (sptr) arg0->m_pOwner->m_AI.m_pParent;
#ifdef TARGET_PORT
        ((CPlayer *) temp_s0)->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo(temp_s0, 0x15)] = 1;
#else
        ((Parent0023181C *)((*CPlayer__GetFiringAmmo(temp_s0, 0x15) * 2) + temp_s0))->flags934[0] = 1;
#endif
    }
    temp_s0_2 = arg0->m_pSource;
    if (temp_s0_2 != NULL) {
        temp_a0 = temp_s0_2->header.m_Type;
        if ((temp_a0 == 2) && (temp_s0_2->m_dwFlags & 0x100) && ((temp_v1 = temp_s0_2->header.m_nObjType, (temp_v1 == temp_a0)) || (temp_v1 == 0x111))) {
            CParticleSystem__Free(&gScene.m_ParticleSystem, temp_s0_2);
            CParticle__StopLoopSound(temp_s0_2);
        }
    }
    CParticle__StopLoopSound(arg0);
}
