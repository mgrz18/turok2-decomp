#include "common.h"
#include "structs/game_object.h"

extern f32 D_800B6D28; /* frame delta */

void func_004089C8(void);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);

void func_0025FC08(CGameObjectInstance *self, CAI *ai) {
    CEnemyIntelligence *data = &self->header.m_pIntelligence->m_Enemy;

    if (ai->m_ConditionTimer > 75.0f) {
        ai->m_ConditionTimer = 75.0f;
    }
    ai->m_ConditionTimer -= D_800B6D28;
    if (ai->m_ConditionTimer <= 0.0f) {
        ai->m_Health = 0;
        func_004089C8();
        if (data->m_SideDeathModel != -1) {
            func_00243414(self, ai, 0x38);
        } else {
            func_00243414(self, ai, 0x34);
        }
    }
}
