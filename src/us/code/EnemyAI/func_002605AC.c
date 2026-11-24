#include "common.h"
#include "structs/light.h"
#include "protos/enemy_ai.h"

#include "structs/game_object.h"


/* Host body (C23): the draw callback of a melting corpse (EnemyMode 60). The N64 view reads
 * CAI::m_ModeTime at its N64 0x40 (the host CAI has two pointers before it), and the actor and
 * the draw args came in as s32 (with --high-memory the corpse's draw crashed in
 * CGameObjectInstance__DrawModelImmediate). */
void EnemyAI_DrawFadeOut(sptr arg0, CAI *ai, sptr arg2) {
    f32 var_f1;

    var_f1 = 255.0f - (ai->m_ModeTime * 8.5f);
    gBlendState = 1;
    if (var_f1 < 0.0f) {
        var_f1 = 0.0f;
    }
    gBlendState_m_Color[3] = var_f1;
    CGameObjectInstance__DrawModelImmediate(arg0, (sptr) ai, arg2);
}
