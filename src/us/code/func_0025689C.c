#include "common.h"
#include "structs/game_object.h"

s32 CPlayer__OnParticleHit(sptr, void *, s32, sptr); /* extern */

void CPlayer__OnParticleHitCallback(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    CPlayer__OnParticleHit((sptr) arg0->m_AI.m_pParent, arg0, arg1, arg2);
}
