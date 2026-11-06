#include "common.h"
#include "structs/game_object.h"

s32 AI_IsAimedAt(CGameObjectInstance *obj, CAI *ai, struct CGameObjectInstance *target, f32 angle);
void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void EnemyMode_Evade_Update(CGameObjectInstance *obj, CAI *ai) {
    u16 anim;

    obj->m_dwFlags |= 0x20000;
    anim = ai->m_AnimType;
    if ((anim >= 0x206C && anim <= 0x2070) || (anim >= 0x2076 && anim <= 0x207A) ||
        (anim >= 0x2080 && anim <= 0x2084)) {
        if (AI_IsAimedAt(obj, ai, ai->m_pSightTarget, 0.2617994249f /* 15 deg */)) {
            ai->m_ModeTime = 0;
        } else if (ai->m_ModeTime > 30.0f) {
            CAI__SetMode(obj, ai, 10);
        }
    } else if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 10);
    }
}
