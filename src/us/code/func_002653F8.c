#include "structs/vector.h"
#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204A68.c, func_00204A68.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


typedef struct B01Pair {
    s32 a;
    s32 b;
} B01Pair;

extern u8 D_800F7078[];
void ImpactEvent_Call(CGameObjectInstance *a, CGameObjectInstance *b, s32 kind, CVector3 pos, B01Pair extra);
void CScene__SetInstanceSavedBit(void *mgr, CGameObjectInstance *actor, s32 flag);

static __inline__ s32 r01_check(CGameObjectInstance *actor, CAI *state, f32 scale) {
    if (actor->m_nAnims == 1) return 1;
    if (actor->m_CurrentAnimation.m_nAnimIndex != (s8)state->m_AnimIndex || actor->m_CurrentAnimation.m_nRequestedAnimIndex != (s8)state->m_AnimIndex) return 0;
    if (state->m_AnimRepeatedCount != 0 && !(actor->m_dwFlags & 0x400)) return 1;
    if (actor->m_CurrentAnimation.m_nFrames < 3 && (actor->m_dwFlags & 0x400)) {
        return (f32)actor->m_BlendLength * scale <= actor->m_BlendPercent;
    } else {
        return (f32)actor->m_CurrentAnimation.m_nFrames * scale <= actor->m_CurrentAnimation.m_AnimationFrame;
    }
}

void Door_CloseUpdate(CGameObjectInstance *actor, CAI *state) {
    s32 idx = *(s8 *)((u8 *) actor->header.m_pIntelligence + 0x1D);
    B01Pair extra;
    if (idx != -1) {
        if (r01_check(actor, state, (f32)idx * 0.007874016f)) {
            extra.a = 0;
            ImpactEvent_Call(actor, actor, 7, actor->header.m_vPos, extra);
            CScene__SetInstanceSavedBit(D_800F7078, actor, 0);
        }
    }
}
