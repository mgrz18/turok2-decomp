#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Idle_AF6F0(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimIdle_AF6F0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Idle_AF720(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimIdle_AF720, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Idle_AF750(CGameObjectInstance *obj, sptr arg1) {
    return AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimIdle_AF750, obj->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
}

void EnemyAnim_Alert_AF810(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimAlert_AF810, obj->header.m_pIntelligence->m_Enemy.m_wAlertAnimFlags);
}

void EnemyAnim_Alert_AF840(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimAlert_AF840, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wAlertAnimFlags);
}

void EnemyAnim_Alert_AF870(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimAlert_AF870, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wAlertAnimFlags);
}

void EnemyAnim_TakeCover_AF8A0(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimTakeCover_AF8A0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}

void EnemyAnim_TakeCover_AF8D0(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimTakeCover_AF8D0, obj->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}

void EnemyAnim_TakeCover_AF900(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimTakeCover_AF900, obj->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}

void EnemyAnim_LeftArmWound_AF930(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimLeftArmWound_AF930, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wLeftArmWoundAnimFlags);
}

void EnemyAnim_LeftArmWound_AF960(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimLeftArmWound_AF960, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wLeftArmWoundAnimFlags);
}

void EnemyAnim_LeftArmWound_AF990(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimLeftArmWound_AF990, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wLeftArmWoundAnimFlags);
}

void EnemyAnim_RightArmWound_AF9C0(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimRightArmWound_AF9C0, obj->header.m_pIntelligence->m_Enemy.m_wRightArmWoundAnimFlags);
}

void EnemyAnim_RightArmWound_AF9F0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimRightArmWound_AF9F0, obj->header.m_pIntelligence->m_Enemy.m_wRightArmWoundAnimFlags);
}

void EnemyAnim_RightArmWound_AFA20(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimRightArmWound_AFA20, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wRightArmWoundAnimFlags);
}

void EnemyAnim_Patrol_AFA50(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimPatrol_AFA50, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wPatrolAnimFlags);
}

void EnemyAnim_Patrol_AFA80(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimPatrol_AFA80, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wPatrolAnimFlags);
}

void EnemyAnim_Patrol_AFAB0(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimPatrol_AFAB0, obj->header.m_pIntelligence->m_Enemy.m_wPatrolAnimFlags);
}

void EnemyAnim_Move_AFAE0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimMove_AFAE0, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_AFB68(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMove_AFB68, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_AFBF0(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMove_AFBF0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_AFC78(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMove_AFC78, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_AFCA8(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimMove_AFCA8, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_AFCD8(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimMove_AFCD8, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Flee_AFD08(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimFlee_AFD08, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wFleeAnimFlags);
}

void EnemyAnim_Flee_AFD38(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimFlee_AFD38, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wFleeAnimFlags);
}

void EnemyAnim_Flee_AFD68(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimFlee_AFD68, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wFleeAnimFlags);
}

s32 WeightedTable_Add(sptr, s32, s32);
s16 WeightedTable_Pick(sptr); /* ABI: returns s16 */
s32 EnemyAnim_Combat_AFE28(CGameObjectInstance *arg0, sptr arg1);
s32 EnemyAnim_Leap_AFEB8(CGameObjectInstance *obj, CAI *ai);
s32 EnemyAnim_Weapon_B0068(CGameObjectInstance *obj, sptr arg1);
void WeightedTable_Clear(void *);

/* host body (round 8 w2): the mode-table picker of the index-0 attacks; the N64 text below reads
 * the definition at the N64 0x14 of the instance (the first enemy melee crashed here). The
 * combat, leap and weapon animation flags are the body's 0x6A, 0x6C and 0x72. */
s16 EnemyAnim_PickAttack_B0A60(CGameObjectInstance *arg0, sptr arg1) {
    u8 sp10[0x68];
    CEnemyIntelligence *info = &arg0->header.m_pIntelligence->m_Enemy;

    WeightedTable_Clear(sp10);
    if (info->m_wCombatAnimFlags & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Combat_AFE28(arg0, arg1), 0xA);
    }
    if (info->m_wLeapAnimFlags & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Leap_AFEB8(arg0, arg1), 0xA);
    }
    if (info->m_wWeaponAnimFlags & 0x1F) {
        WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Weapon_B0068(arg0, arg1), 0xA);
    }
    return WeightedTable_Pick((sptr) sp10);
}
s32 EnemyAnim_Combat_AFE58(CGameObjectInstance *arg0, sptr arg1);
s32 EnemyAnim_Leap_AFEE8(CGameObjectInstance *arg0, sptr arg1);
s32 EnemyAnim_Weapon_B00C0(CGameObjectInstance *obj, CAI *ai);

s16 EnemyAnim_PickAttack_B0E80(sptr arg0, sptr arg1) {
    u8 sp10[0x68];

    WeightedTable_Clear(sp10);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Combat_AFE58(arg0, arg1), 0xA);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Leap_AFEE8(arg0, arg1), 0xA);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Weapon_B00C0(arg0, arg1), 0xA);
    return WeightedTable_Pick((sptr) sp10);
}
s32 EnemyAnim_Combat_AFE88(CGameObjectInstance *obj, sptr arg1);
s32 EnemyAnim_Leap_AFF18(CGameObjectInstance *arg0, sptr arg1);
s32 EnemyAnim_Weapon_B00F0(CGameObjectInstance *arg0, sptr arg1);

s16 EnemyAnim_PickAttack_B1200(sptr arg0, sptr arg1) {
    u8 sp10[0x68];

    WeightedTable_Clear(sp10);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Combat_AFE88(arg0, arg1), 0xA);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Leap_AFF18(arg0, arg1), 0xA);
    WeightedTable_Add((sptr) sp10, (s32) (s16) EnemyAnim_Weapon_B00F0(arg0, arg1), 0xA);
    return WeightedTable_Pick((sptr) sp10);
}

void EnemyAnim_ComboEnd_AFD98(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimComboEnd_AFD98, obj->header.m_pIntelligence->m_Enemy.m_wComboEndAnimFlags);
}

void EnemyAnim_ComboEnd_AFDC8(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimComboEnd_AFDC8, obj->header.m_pIntelligence->m_Enemy.m_wComboEndAnimFlags);
}

void EnemyAnim_ComboEnd_AFDF8(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimComboEnd_AFDF8, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wComboEndAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Combat_AFE28(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimCombat_AFE28, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wCombatAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Combat_AFE58(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimCombat_AFE58, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wCombatAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Combat_AFE88(CGameObjectInstance *obj, sptr arg1) {
    return AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimCombat_AFE88, obj->header.m_pIntelligence->m_Enemy.m_wCombatAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Leap_AFEB8(CGameObjectInstance *obj, CAI *ai) {
    return AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimLeap_AFEB8, obj->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Leap_AFEE8(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimLeap_AFEE8, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Leap_AFF18(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimLeap_AFF18, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}

void EnemyAnim_Dart_AFF48(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimDart_AFF48, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wDartAnimFlags);
}

void EnemyAnim_Dart_AFF78(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimDart_AFF78, obj->header.m_pIntelligence->m_Enemy.m_wDartAnimFlags);
}

void EnemyAnim_DartFallback_AFF18(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimLeap_AFF18, obj->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}

void EnemyAnim_Projectile_AFFD8(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimProjectile_AFFD8, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wProjectileAnimFlags);
}

void EnemyAnim_Projectile_B0008(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimProjectile_B0008, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wProjectileAnimFlags);
}

void EnemyAnim_ProjectileFallback_AFF18(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimLeap_AFF18, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Weapon_B0068(CGameObjectInstance *obj, sptr arg1) {
    return AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimWeapon_B0068, obj->header.m_pIntelligence->m_Enemy.m_wWeaponAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Weapon_B00C0(CGameObjectInstance *obj, CAI *ai) {
    return AI_PickAnimFromTable(obj, ai, sEnemyAnimWeapon_B00C0, obj->header.m_pIntelligence->m_Enemy.m_wWeaponAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_Weapon_B00F0(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimWeapon_B00F0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wWeaponAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_NormalDeath_B0120(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimNormalDeath_B0120, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_NormalDeath_B0150(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimNormalDeath_B0150, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_NormalDeath_B0180(CGameObjectInstance *obj, sptr arg1) {
    return AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimNormalDeath_B0180, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_MovingDeath_B01B0(CGameObjectInstance *obj, CAI *ai) {
    return AI_PickAnimFromTable(obj, ai, sEnemyAnimMovingDeath_B01B0, obj->header.m_pIntelligence->m_Enemy.m_wMovingDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_MovingDeath_B01E0(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMovingDeath_B01E0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMovingDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_MovingDeath_B0210(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMovingDeath_B0210, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMovingDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_ViolentDeath_B0240(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimViolentDeath_B0240, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_ViolentDeath_B0270(CGameObjectInstance *obj, sptr arg1) {
    return AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimViolentDeath_B0270, obj->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_ViolentDeath_B02A0(CGameObjectInstance *obj, CAI *ai) {
    return AI_PickAnimFromTable(obj, ai, sEnemyAnimViolentDeath_B02A0, obj->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_ExplosiveDeath_B02D0(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimExplosiveDeath_B02D0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wExplosiveDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_ExplosiveDeath_B0300(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimExplosiveDeath_B0300, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wExplosiveDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_ExplosiveDeath_B0330(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimExplosiveDeath_B0330, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wExplosiveDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_NormalDeath_B0360(CGameObjectInstance *obj, sptr arg1) {
    return AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimNormalDeath_B0360, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_NormalDeath_B0390(CGameObjectInstance *obj, CAI *ai) {
    return AI_PickAnimFromTable(obj, ai, sEnemyAnimNormalDeath_B0390, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 EnemyAnim_NormalDeath_B03C0(CGameObjectInstance *arg0, sptr arg1) {
    return AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimNormalDeath_B03C0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}

void EnemyAnim_Evade_B03F0(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimEvade_B03F0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}

void EnemyAnim_Evade_B0420(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimEvade_B0420, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}

void EnemyAnim_Evade_B0450(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimEvade_B0450, obj->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}

void EnemyAnim_Evade_B0480(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimEvade_B0480, obj->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}

void EnemyAnim_Evade_B04B0(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimEvade_B04B0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}

void EnemyAnim_Evade_B04E0(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimEvade_B04E0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}

void EnemyAnim_Move_B0510(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMove_B0510, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_B0540(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimMove_B0540, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_B0570(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimMove_B0570, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_B05A0(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMove_B05A0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

#include "structs/intelligence.h"
#include "globals/controller.h"

/* Three override slots, 6 bytes apart. */

s32 EnemyAnim_JoypadTestPlayAnim(CGameObjectInstance *actor, void *state) {
    u16 *slots = &gContPadData[1].button;
    s32 r;

    if (slots[0] != 0) {
        r = AI_PickAnimFromTable(actor, state, sJoypadTestAnimTable, slots[0]);
    } else if (slots[3] != 0) {
        r = AI_PickAnimFromTable(actor, state, sJoypadTestAnimTable, slots[3]) + 100;
    } else if (slots[6] == 0) {
        r = AI_PickAnimFromTable(actor, state, (u8 *) sEnemyAnimIdle_AF6F0, actor->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
    } else {
        r = AI_PickAnimFromTable(actor, state, sJoypadTestAnimTable, slots[6]) + 200;
    }
    return r;
}

void EnemyAnim_Move_B05D0(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMove_B05D0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}

void EnemyAnim_Move_B0600(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimMove_B0600, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}
