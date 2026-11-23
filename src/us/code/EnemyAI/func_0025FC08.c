#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"
#include "protos/ai.h"


void TotemDefense_AddKill(void);

void EnemyMode_ReactionBore_Update(CGameObjectInstance *self, CAI *ai) {
    CEnemyIntelligence *data = &self->header.m_pIntelligence->m_Enemy;

    if (ai->m_ConditionTimer > 75.0f) {
        ai->m_ConditionTimer = 75.0f;
    }
    ai->m_ConditionTimer -= gFrameIncrement;
    if (ai->m_ConditionTimer <= 0.0f) {
        ai->m_Health = 0;
        TotemDefense_AddKill();
        if (data->m_SideDeathModel != -1) {
            CAI__SetMode(self, ai, 0x38);
        } else {
            CAI__SetMode(self, ai, 0x34);
        }
    }
}
