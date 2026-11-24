#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"

/* Whether the current behaviour (m_Enemy.m_BehaviorIndex, 0-2) has an animation in the body's
 * flags at 0x7E (m_wAlertAnimFlags: five bits per behaviour); never in modes 0x20 and 0x3D.
 * One body for the N64 and the port since round 29 (the N64 text read the definition and the
 * AI at their N64 offsets, so the port had its own copy). */
s32 EnemyAI_CurrentHasComboEndAnim(CGameObjectInstance *arg0, CAI *arg1) {
    CEnemyIntelligence *p = &arg0->header.m_pIntelligence->m_Enemy;
    s8 c = arg1->m_Mode;

    if (c == 0x20 || c == 0x3D) {
        return 0;
    }
    switch (arg1->m_Enemy.m_BehaviorIndex) {
        case 0:
            return (p->m_wAlertAnimFlags & 0x1F) != 0;
        case 1:
            return (p->m_wAlertAnimFlags & 0x3E0) != 0;
        case 2:
            return (p->m_wAlertAnimFlags & 0x7C00) != 0;
    }
    return 0;
}
