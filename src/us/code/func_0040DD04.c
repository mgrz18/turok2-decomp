#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void BlindOne_BossFlinchUpdate(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount >= 7) {
        CAI__SetMode(obj, ai, 7);
    }
}
