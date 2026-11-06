#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

void EnemyMode_FeignDeath_Update(s32 arg0, CAI *ai) {
    s32 var_a2;

    if (ai->m_AnimRepeatedCount != 0) {
        var_a2 = 0x3C;
        if (ai->m_Health != 0) {
            var_a2 = 0x1E;
        }
        CAI__SetMode(arg0, (s32) ai, var_a2);
    }
}
