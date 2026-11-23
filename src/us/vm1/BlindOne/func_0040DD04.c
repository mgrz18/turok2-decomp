#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void BlindOne_BossFlinchUpdate(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount >= 7) {
        CAI__SetMode(obj, ai, 7);
    }
}
