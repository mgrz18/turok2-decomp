#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void BlindOne_ActorAttack4Update_2(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 2);
    }
}
