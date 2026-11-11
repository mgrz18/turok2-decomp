#include "common.h"
#include "structs/game_object.h"

s32 CPlayer__OnParticleHit(sptr, void *, sptr, sptr); /* extern */

void CPlayer__OnParticleHitCallback(CGameObjectInstance *arg0, sptr arg1, sptr arg2) {
    CPlayer__OnParticleHit((sptr) arg0->m_AI.m_pParent, arg0, arg1, arg2);
}
