#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"


void EnemyMode_ReactionGetup_Update(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 0xA);
    }
}
