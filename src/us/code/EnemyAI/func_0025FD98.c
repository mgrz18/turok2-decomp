#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "protos/enemy_ai.h"

/* De-faked in round 21 (j4): the N64 draft tested the CAI pointer in two identical arms (which
 * kept the beq's delay slot empty) and had a separate host body; the test on mode 0x35 first gives
 * the ROM's layout, so the N64 and the host share this body. */

/* The mode-table animation picker of the gib deaths (modes 0x35/0x36): the definition's gib
 * model, + 1 in mode 0x35. */
s32 EnemyAnim_ExtremeDeath(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_ModeBefore == 0x35) {
        return obj->header.m_pIntelligence->m_Enemy.m_GibModel + 1;
    }
    return obj->header.m_pIntelligence->m_Enemy.m_GibModel;
}
