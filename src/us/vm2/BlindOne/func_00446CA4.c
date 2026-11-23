#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void BlindOne_BossMode4Update_2(s32 arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 3);
    }
}
