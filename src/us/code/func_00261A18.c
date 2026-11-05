#include "common.h"
#include "structs/game_object.h"

f32 func_002119FC(f32 lo, f32 hi);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);

void func_00261A18(CGameObjectInstance *self, CAI *ai) {
    CEnemyIntelligence *params;
    CGameObjectInstance *target;
    f32 dist;
    f32 angle;

    params = &self->header.m_pIntelligence->m_Enemy;
    target = ai->m_pPathTarget;
    if (target != NULL && params->m_UnderwaterBehavior != 5) {
        dist = func_002119FC(-102.399994f, 102.399994f) + 512.0f;
        angle = target->m_RotY + 3.1415927f + func_002119FC(-0.78539824f, 0.78539824f);
        ai->m_vLeashPos.x = target->header.m_vPos.x + dist * func_002982F0(angle);
        ai->m_vInitialPos.y = (ai->m_vLeashPos.y + target->header.m_vPos.y) * 0.5f + func_002119FC(-51.199997f, 51.199997f);
        ai->m_vLeashPos.z = target->header.m_vPos.z + dist * func_002974C0(angle);
    }
    if (params->m_UnderwaterBehavior == 4 && ai->m_ModeBefore != 27) {
        func_00243414(self, ai, 27);
    } else if (params->m_UnderwaterBehavior == 3 && ai->m_ModeBefore != 10) {
        func_00243414(self, ai, 10);
    }
}
