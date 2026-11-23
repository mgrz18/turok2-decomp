#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_FeignDeath_Update(sptr arg0, CAI *ai) {
    s32 var_a2;

    if (ai->m_AnimRepeatedCount != 0) {
        var_a2 = 0x3C;
        if (ai->m_Health != 0) {
            var_a2 = 0x1E;
        }
        CAI__SetMode(arg0, ai, var_a2);
    }
}
