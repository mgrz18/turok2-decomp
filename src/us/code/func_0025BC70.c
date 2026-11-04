#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* AI init for a family of actors: installs the AI callbacks and seeds the
 * timers/flags from the actor definition. Types 0x44E..0x456 (except 0x44F and
 * 0x452) are left alone. */

extern struct { u8 value; } D_8011AAF1;
extern u8 D_800B0A60[];
extern u8 D_800F7078[];

extern void func_0025C5F8();
extern void func_0025C018();
extern void func_0025BA80();
extern void func_0025CAC8();
extern void func_0025EF68();
extern void func_0025B2D4();
extern void func_0025E4B8();
extern void func_00220260(void *mgr, CGameObjectInstance *actor, s32 flag);
extern f32 func_002119FC(f32 lo, f32 hi);

void func_0025BC70(CGameObjectInstance *actor, CAI *ai) {
    CEnemyIntelligence *info;
    s32 mode2;
    s32 one; /* the mode indices live in variables: constants do not match */
    s32 two;

    actor->m_dwFlags |= 0x02000000;
    actor->m_dwFlags |= 0x04000000;
    info = &actor->header.m_pIntelligence->m_Enemy;
    if (D_8011AAF1.value == 0) {
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
    ai->m_pModeTable = D_800B0A60;
    ai->m_pfStep = func_0025C5F8;
    ai->m_pfCalculateOrientationMatrix = func_0025C018;
    ai->m_pfDraw = func_0025BA80;
    if (actor->m_nTypeFlag == 0x136) {
        ai->m_pfHit = NULL;
        ai->m_pfCollision = NULL;
        ai->m_pfHurt = NULL;
        func_00220260(D_800F7078, actor, 0);
    } else {
        ai->m_pfHit = func_0025CAC8;
        ai->m_pfCollision = func_0025EF68;
        ai->m_pfHurt = func_0025B2D4;
    }
    ai->m_VarS32[0] = 0;
    ai->m_VarS32[1] = 0;
    ai->m_VarS32[2] = 0;
    ai->m_VarS32[3] = 0;
    ai->m_Enemy.m_AttackDelay = 0;
    ai->m_VarS32[6] = 0;
    ai->m_VarS32[7] = 0;
    ai->m_VarF32[4] = 1.0f;
    ai->m_Enemy.m_IdleTimer = func_002119FC(60.0f, 120.0f);
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
    ai->m_pfEffects = func_0025E4B8;
    if (!(actor->m_dwFlags & 0x80000) && (info->m_dwFlags & 0x1000)) {
        actor->m_dwFlags &= ~0x2000;
        actor->m_dwFlags &= ~0x100;
    }
    if (!(info->m_dwFlags & 0x80000)) {
        actor->m_dwFlags |= 0x01000000;
    }
}
