#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "globals/ai.h"

/* AI init for a family of actors: installs the AI callbacks and seeds the
 * timers/flags from the actor definition. Types 0x44E..0x456 (except 0x44F and
 * 0x452) are left alone. */

extern struct { u8 value; } gEngineApp_m_Settings_multiplayer;
extern u8 gScene[];

extern void EnemyAI_Update();
extern void EnemyAI_CalcOrientationMatrix();
extern void EnemyAI_DrawFaded();
extern void EnemyAI_OnHit();
extern void EnemyAI_CheckGrab();
extern void EnemyAI_OnDamage();
extern void EnemyAI_HeadNodeCallback();
extern void CScene__SetInstanceSavedBit(void *mgr, CGameObjectInstance *actor, s32 flag);
extern f32 Random_Blend(f32 lo, f32 hi);

void EnemyAI_Init(CGameObjectInstance *actor, CAI *ai) {
    CEnemyIntelligence *info;
    s32 mode2;
    s32 one; /* the mode indices live in variables: constants do not match */
    s32 two;

    actor->m_dwFlags |= 0x02000000;
    actor->m_dwFlags |= 0x04000000;
    info = &actor->header.m_pIntelligence->m_Enemy;
    if (gEngineApp_m_Settings_multiplayer.value == 0) {
        actor->m_dwFlags |= 0x10000000;
    }
    switch (actor->m_nTypeFlag) {
    case 0x44E:
    case 0x450:
    case 0x451:
    case 0x453:
    case 0x454:
    case 0x455:
    case 0x456:
        return;
    }
    ai->m_pModeTable = gEnemyGroundModeTable;
    ai->m_pfStep = EnemyAI_Update;
    ai->m_pfCalculateOrientationMatrix = EnemyAI_CalcOrientationMatrix;
    ai->m_pfDraw = EnemyAI_DrawFaded;
    if (actor->m_nTypeFlag == 0x136) {
        ai->m_pfHit = NULL;
        ai->m_pfCollision = NULL;
        ai->m_pfHurt = NULL;
        CScene__SetInstanceSavedBit(gScene, actor, 0);
    } else {
        ai->m_pfHit = EnemyAI_OnHit;
        ai->m_pfCollision = EnemyAI_CheckGrab;
        ai->m_pfHurt = EnemyAI_OnDamage;
    }
    ai->m_Enemy.m_Pitch = 0;
    ai->m_Enemy.m_Roll = 0;
    ai->m_Enemy.m_pGrabbed = NULL;
    ai->m_Enemy.m_TurnRate = 0;
    ai->m_Enemy.m_AttackDelay = 0;
    ai->m_Enemy.unk18 = 0;
    ai->m_Enemy.m_WanderTimer = 0;
    ai->m_Enemy.m_BaseScale = 1.0f;
    ai->m_Enemy.m_IdleTimer = Random_Blend(60.0f, 120.0f);
    ai->m_Enemy.m_Timer = 0;
    ai->m_Enemy.m_Timer2 = 150.0f;
    ai->m_Enemy.m_Behavior[0] = info->m_Behaviors[0];
    one = 1;
    ai->m_Enemy.m_Behavior[one] = info->m_Behaviors[one];
    ai->m_Enemy.m_Behavior[2] = mode2 = info->m_UnderwaterBehavior;
    if (ai->m_Enemy.m_Behavior[0]) {
        ai->m_Enemy.m_BehaviorIndex = 0;
    } else if (ai->m_Enemy.m_Behavior[one]) {
        ai->m_Enemy.m_BehaviorIndex = one;
    } else {
        two = 2;
        if (mode2) {
            ai->m_Enemy.m_BehaviorIndex = two;
        } else {
            ai->m_Enemy.m_BehaviorIndex = 0;
        }
    }
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    if (!(actor->m_dwFlags & 0x80000) && (info->m_dwFlags & 0x1000)) {
        actor->m_dwFlags &= ~0x2000;
        actor->m_dwFlags &= ~0x100;
    }
    if (!(info->m_dwFlags & 0x80000)) {
        actor->m_dwFlags |= 0x01000000;
    }
}
