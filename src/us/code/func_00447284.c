#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void BlindOne_ActorAttack3Update_2(s32 arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, (s32) ai, 2);
    }
}
