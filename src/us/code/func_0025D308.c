#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* AI dodge check: if the definition enables dodging for the current mode,
 * dodges with a probability that grows with the AI's skill (0..255), to the
 * side away from the threat (animation 8 or 9). */

extern s32 func_002113A4(void);
extern s32 func_002451AC(CGameObjectInstance *actor, CAI *ai, s32 threat, f32 angle);
extern void func_00243414(CGameObjectInstance *actor, CAI *ai, s32 anim);

void func_0025D308(CGameObjectInstance *actor, CAI *ai) {
    f32 skill;
    s32 chance;
    s32 side;
    s32 mode;
    s32 mode2;

    mode = ai->m_VarS8[39];
    mode2 = mode;
    if ((mode == 0 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x1F)) || (mode == 1 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x3E0))
        || (mode2 == 2 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x7C00))) {
        skill = ai->m_Aggression * (1.0f / 255);
        chance = skill * (skill * 600.0f) + 100.0f;
        if (func_002113A4() % 10000 < chance) {
            side = func_002451AC(actor, ai, ai->m_pSightTarget, 0.261799425f);
            if (side == -1) {
                func_00243414(actor, ai, 8);
            } else if (side == 1) {
                func_00243414(actor, ai, 9);
            }
        }
    }
}
