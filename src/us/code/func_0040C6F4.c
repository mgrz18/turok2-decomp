#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern u8 sMotherBossPhase1PlayerPos[];
extern void AI_UpdateTargets(CGameObjectInstance *obj, void *ai, CAITargets *t, s32 f1, s32 f2);
extern void CPlayer__SetPosition(CPlayer *player, void *arg1);
extern void CPlayer__UpdateOrbitCamera(CPlayer *player, CPlayer *other);
extern void CAI__SetMode(CGameObjectInstance *obj, void *ai, s32 state);

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist(CGameObjectInstance *obj, CGameObjectInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = t.m_Path.m_pTarget;
    }
    return dist;
}

void MotherBoss_OnPhase1CinemaDone(void) {
    CGameObjectInstance *obj = gMotherBoss.obj;
    CGameObjectInstance *found;
    CPlayer *player;

    player = NULL;
    getTargetDist(obj, &found);
    if (found != NULL) {
        player = ((CPlayer *)found->m_AI.m_pParent);
    }
    if (player != NULL) {
        CPlayer__SetPosition(player, sMotherBossPhase1PlayerPos);
        CPlayer__UpdateOrbitCamera(player, player);
        player->m_WeaponActor.m_RotY = player->m_CharacterActor.m_RotY = 3.141593f;
        obj->m_RotY = 0.0f;
    }
    CAI__SetMode(obj, &obj->m_AI, 0xA);
}
