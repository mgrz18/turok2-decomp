#include "common.h"
#include "structs/game_object.h"

/* Lowers the aggression by arg2, down to 0. */
void EnemyAI_LowerAggression(s32 arg0, CAI *arg1, s32 arg2) {
    s32 var_v0;

    var_v0 = arg1->m_Aggression - arg2;
    if (var_v0 < 0) {
        var_v0 = 0;
    }
    arg1->m_Aggression = (u8) var_v0;
}
