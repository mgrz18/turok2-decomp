#include "common.h"
#include "structs/game_object.h"

s32 CPlayer__OnParticleHit(s32, void *, s32, s32); /* extern */

void CPlayer__OnParticleHitCallback(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    CPlayer__OnParticleHit((s32) arg0->m_AI.m_pParent, arg0, arg1, arg2);
}
