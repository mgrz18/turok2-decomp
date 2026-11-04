#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"

/* AI attack-wait callback: state 0 counts the delay down; once it has run out
 * (more than 30 frames left means an attack is due) it plays the attack
 * animation for the current mode if the definition allows it. State 1 counts
 * the cool-down and re-picks a region. */

extern f32 D_800B6D28; /* frame delta */
extern CVector3 D_800B06A0;
extern CVector3 D_800B06AC;
extern u8 D_800F7078[];

extern void func_00243414(CGameObjectInstance *actor, CAI *ai, s32 anim);
extern s16 func_002261A8(void *level, s32 id, CVector3 *pos);

static __inline__ s32 canAttack(CAI *ai, CEnemyIntelligence *info, s32 state) {
    if (state == 0x20 || state == 0x3D) {
        return 0;
    }
    switch (ai->m_Enemy.m_BehaviorIndex) {
    case 0:
        return (info->m_wAlertAnimFlags & 0x1F) != 0;
    case 1:
        return (info->m_wAlertAnimFlags & 0x3E0) != 0;
    case 2:
        return (info->m_wAlertAnimFlags & 0x7C00) != 0;
    }
    return 0;
}

void func_0025C430(CGameObjectInstance *actor, CAI *ai) {
    s32 state;

    if (ai->m_Health == 0) {
        return;
    }
    state = ai->m_Mode;
    if (state == 0) {
        return;
    }
    switch (ai->m_Action) {
    case 0:
        if (ai->m_ActionTime > 30.0f) {
            if (canAttack(ai, &actor->header.m_pIntelligence->m_Enemy, state)) {
                func_00243414(actor, ai, 0xC);
            }
            D_800B06A0 = D_800B06AC;
            ai->m_Action = 1;
        } else if (ai->m_ActionTime > 0.0f) {
            ai->m_ActionTime -= D_800B6D28;
            if (ai->m_ActionTime < 0.0f) {
                ai->m_ActionTime = 0.0f;
            }
        }
        ai->m_Enemy.m_AttackDelay = 0;
        break;
    case 1:
        ai->m_ActionTime -= D_800B6D28;
        if (ai->m_ActionTime <= 0.0f) {
            ai->m_ActionTime = 0.0f;
            ai->m_Action = 0;
            if (ai->m_PathTrack.m_Mode) {
                ai->m_PathTrack.m_nCurrentPointIndex = func_002261A8(D_800F7078, ai->m_PathTrack.m_nCurrentPathIndex, &actor->header.m_vPos);
            }
        }
        break;
    }
}
