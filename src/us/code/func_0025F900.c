#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_ReactionExplosion_Update(s32 arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, (s32) ai, 0x1E);
    }
}
