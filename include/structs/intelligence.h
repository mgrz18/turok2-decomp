#ifndef STRUCTS_INTELLIGENCE_H
#define STRUCTS_INTELLIGENCE_H

/* An actor's intelligence: the definition its instance points at
 * (CGameObjectInstance.header.m_pIntelligence), loaded with the level from
 * the cartridge's intelligence set. A common header, then from 0x14 a body
 * whose layout depends on m_Type.
 *
 * Names from Rage Wars' debug type records (CIntelligence_t and the
 * C*Intelligence_t bodies; references/rw_abu: src/DATA/OVLTBL_9/800F8202_2.CPP
 * and src/IO/80200568_3.CPP), the same engine. Turok 2's bodies line up with
 * them field for field, except that its CCommonEnemyIntelligence is 0x34
 * bytes, not 0x38, so what follows it (the enemy and turret fields) sits 4
 * bytes lower. Checked against the code: the health setup (func_002427E0)
 * reads the enemy's and the turret's health as a word at 0x18 and the
 * destructible's as a halfword, and hands 0x40/0x41 to the AI's m_Aggression
 * and m_TranqHealth; the generator's drop (func_00262D74) and delay
 * (func_00263594) read its object, variation, scale, particle and intervals;
 * the destructible's accessors (func_00265B2C...) its animations; the enemy's
 * 19 animation masks (handed to func_00245A98, one bit per animation of an
 * action table) run from 0x76 to 0x9A in Rage Wars' order (evade, combat
 * and the four attacks paired with the five attack radii, the arm wounds
 * as the hit-region reactions, flee as "can run").
 *
 * Offsets in the bodies are from the body (0x14); the comments give the
 * offset in CIntelligence too. Fields named unk are not yet known. */

#include "common.h"

/* m_Type, in Rage Wars' order (which Turok 2 follows: 1 and 4 read the
 * common enemy block, 7 a halfword health, 11 is the player's own). */
enum {
    INTELLIGENCE_STATIC,            /* 0 */
    INTELLIGENCE_ENEMY,             /* 1 */
    INTELLIGENCE_PLATFORM,          /* 2 */
    INTELLIGENCE_GENERATOR,         /* 3 */
    INTELLIGENCE_TURRET,            /* 4 */
    INTELLIGENCE_THROWABLE,         /* 5 */
    INTELLIGENCE_DOOR,              /* 6 */
    INTELLIGENCE_DESTRUCTIBLE,      /* 7 */
    INTELLIGENCE_PICKUP,            /* 8 */
    INTELLIGENCE_INTERACTIVE_ANIM,  /* 9 */
    INTELLIGENCE_ACTION,            /* 10 */
    INTELLIGENCE_PLAYER,            /* 11 */
    INTELLIGENCE_MORPHER            /* 12 */
};

/* Type 1, the enemies (CCommonEnemyIntelligence + CEnemyIntelligence). The
 * first 0x34 bytes are shared with the turret (type 4). */
typedef struct CEnemyIntelligence {
    /* 0x00 (0x14) */ u32 m_dwFlags;
    /* 0x04 (0x18) */ s32 m_Health;
    /* 0x08 (0x1C) */ s32 m_StartAnim;
    /* 0x0C (0x20) */ s16 unk0C;
    /* 0x0E (0x22) */ u8 pad0E[2];
    /* 0x10 (0x24) */ s32 unk10;
    /* 0x14 (0x28) */ f32 m_CollisionRadius;
    /* 0x18 (0x2C) */ f32 m_CollisionWallRadius;
    /* 0x1C (0x30) */ f32 m_CollisionHeight;
    /* 0x20 (0x34) */ f32 m_CollisionDeadHeight;
    /* 0x24 (0x38) */ u32 m_dwFlags2;
    /* 0x28 (0x3C) */ f32 m_LeashRadius;
    /* 0x2C (0x40) */ u8 m_Aggression;
    /* 0x2D (0x41) */ u8 m_TranqHealth;      /* also the flinch countdown's reload */
    /* 0x2E (0x42) */ u16 m_StartSound;
    /* 0x30 (0x44) */ u16 m_PainSound;
    /* 0x32 (0x46) */ u8 pad32[2];
    /* 0x34 (0x48) */ u32 m_dwEnemyFlags;    /* 0x40/0x80/0x100: may attack side 0/1/2 */
    /* 0x38 (0x4C) */ f32 m_AttackCombatRadius;
    /* 0x3C (0x50) */ f32 m_AttackLeapRadius;
    /* 0x40 (0x54) */ f32 m_AttackDartRadius;
    /* 0x44 (0x58) */ f32 m_AttackProjectileRadius;
    /* 0x48 (0x5C) */ f32 m_AttackWeaponRadius;
    /* 0x4C (0x60) */ u8 m_GroundBehavior;
    /* 0x4D (0x61) */ u8 m_AirBehavior;
    /* 0x4E (0x62) */ u8 m_UnderwaterBehavior;
    /* 0x4F (0x63) */ u8 pad4F[0x58 - 0x4F];
    /* 0x58 (0x6C) */ s16 m_DeathAnims[5];
    /* 0x62 (0x76) */ u16 m_wIdleAnimFlags;
    /* 0x64 (0x78) */ u16 m_wMoveAnimFlags;
    /* 0x66 (0x7A) */ u16 m_wPatrolAnimFlags;
    /* 0x68 (0x7C) */ u16 m_wEvadeAnimFlags;
    /* 0x6A (0x7E) */ u16 m_wCombatAnimFlags;
    /* 0x6C (0x80) */ u16 m_wLeapAnimFlags;
    /* 0x6E (0x82) */ u16 m_wDartAnimFlags;
    /* 0x70 (0x84) */ u16 m_wProjectileAnimFlags;
    /* 0x72 (0x86) */ u16 m_wWeaponAnimFlags;
    /* 0x74 (0x88) */ u16 m_wComboEndAnimFlags;
    /* 0x76 (0x8A) */ u16 m_wNormalDeathAnimFlags;
    /* 0x78 (0x8C) */ u16 m_wMovingDeathAnimFlags;
    /* 0x7A (0x8E) */ u16 m_wViolentDeathAnimFlags;
    /* 0x7C (0x90) */ u16 m_wExplosiveDeathAnimFlags;
    /* 0x7E (0x92) */ u16 m_wAlertAnimFlags;
    /* 0x80 (0x94) */ u16 m_wTakeCoverAnimFlags;
    /* 0x82 (0x96) */ u16 m_wLeftArmWoundAnimFlags;
    /* 0x84 (0x98) */ u16 m_wRightArmWoundAnimFlags;
    /* 0x86 (0x9A) */ u16 m_wFleeAnimFlags;
} CEnemyIntelligence; /* size 0x88 */

/* Type 3: spawns objects or particle effects. */
typedef struct CGeneratorIntelligence {
    /* 0x00 (0x14) */ u32 m_dwFlags;
    /* 0x04 (0x18) */ s32 m_Type;            /* 0 object, 1 particle effect, 2 random pickup */
    /* 0x08 (0x1C) */ s16 m_FiniteTotal;
    /* 0x0A (0x1E) */ s16 m_MaxActive;
    /* 0x0C (0x20) */ f32 m_IntervalTime1;
    /* 0x10 (0x24) */ f32 m_IntervalTime2;
    /* 0x14 (0x28) */ s16 m_ObjectIndex;
    /* 0x16 (0x2A) */ s16 m_VariationIndex;
    /* 0x18 (0x2C) */ f32 m_ScaleX;
    /* 0x1C (0x30) */ f32 m_ScaleY;
    /* 0x20 (0x34) */ f32 m_ScaleZ;
    /* 0x24 (0x38) */ s16 m_ParticleType;
    /* 0x26 (0x3A) */ s16 m_Unused;
} CGeneratorIntelligence; /* size 0x28 */

/* Type 4: shares the enemies' first 0x34 bytes. */
typedef struct CTurretIntelligence {
    /* 0x00 (0x14) */ u8 m_Common[0x34];
    /* 0x34 (0x48) */ u32 m_dwTurretFlags;
    /* 0x38 (0x4C) */ s32 m_GunNode;         /* event ids the turret's animation fires */
    /* 0x3C (0x50) */ s32 m_RotNode;
    /* 0x40 (0x54) */ f32 m_RotSpeed;
} CTurretIntelligence;

/* Type 6. */
typedef struct CDoorIntelligence {
    /* 0x00 (0x14) */ u32 m_dwFlags;
    /* 0x04 (0x18) */ f32 m_OpenDuration;
    /* 0x08 (0x1C) */ s8 m_OpenDoorEvent;     /* in 1/127ths of the animation, -1: none */
    /* 0x09 (0x1D) */ s8 m_CloseDoorEvent;
    /* 0x0A (0x1E) */ u8 pad0A[2];
    /* 0x0C (0x20) */ f32 m_AutoOpenRadius;
    /* 0x10 (0x24) */ f32 m_AutoCloseRadius;
} CDoorIntelligence; /* size 0x14 */

/* Type 7: breakable objects (and the totems). */
typedef struct CDestructibleIntelligence {
    /* 0x00 (0x14) */ u32 m_dwFlags;
    /* 0x04 (0x18) */ s16 m_Health;
    /* 0x06 (0x1A) */ s16 pad06;
    /* 0x08 (0x1C) */ f32 m_CollisionRadius;
    /* 0x0C (0x20) */ f32 m_CollisionHeight;
    /* 0x10 (0x24) */ s32 m_IdleAnim;
    /* 0x14 (0x28) */ f32 m_AutoGoRadius;
    /* 0x18 (0x2C) */ s32 m_GoAnim;
    /* 0x1C (0x30) */ s32 m_GoParticleEffect;
    /* 0x20 (0x34) */ s32 m_GoSoundEffect;
    /* 0x24 (0x38) */ u32 m_dwGoPickupsFlag1;
    /* 0x28 (0x3C) */ u32 m_dwGoPickupsFlag2;
    /* 0x2C (0x40) */ s32 m_GoDeathAnim;
    /* 0x30 (0x44) */ s32 m_GoDeathParticleEffect;
    /* 0x34 (0x48) */ s32 m_GoDeathSoundEffect;
    /* 0x38 (0x4C) */ u32 m_dwGoDeathPickupsFlag1;
    /* 0x3C (0x50) */ u32 m_dwGoDeathPickupsFlag2;
    /* 0x40 (0x54) */ s32 m_TotemMissionDuration;   /* seconds */
    /* 0x44 (0x58) */ s16 m_EnemiesAttackingTurok;
    /* 0x46 (0x5A) */ s16 m_EnemiesToKillToSaveTotem;
} CDestructibleIntelligence;

/* Type 8. */
typedef struct CPickupIntelligence {
    /* 0x00 (0x14) */ u32 m_dwFlags;
    /* 0x04 (0x18) */ f32 m_CollisionRadius;
    /* 0x08 (0x1C) */ f32 m_CollisionHeight;
    /* 0x0C (0x20) */ f32 m_Time;
} CPickupIntelligence; /* size 0x10 */

/* Type 11: a player character. */
typedef struct CPlayerIntelligence {
    /* 0x00 (0x14) */ u32 m_dwFlags;
    /* 0x04 (0x18) */ u32 m_StartHealth;
} CPlayerIntelligence;

typedef struct CIntelligence {
    /* 0x00 */ s32 m_Type;                   /* INTELLIGENCE_* */
    /* 0x04 */ u32 m_dwFlags;                /* 0x4: ..., 0x100, 0x200: the views it shows in */
    /* 0x08 */ f32 m_LODDist2;
    /* 0x0C */ s16 m_Id;
    /* 0x0E */ s8 m_nModel;
    /* 0x0F */ s8 m_nModel2;
    /* 0x10 */ s8 m_nShadowModel;
    /* 0x11 */ s8 m_nShadowModel2;
    /* 0x12 */ s8 m_nTexture;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ union {
        u32 m_dwTypeFlags;                   /* every body starts with its flags */
        CEnemyIntelligence m_Enemy;
        CGeneratorIntelligence m_Generator;
        CTurretIntelligence m_Turret;
        CDoorIntelligence m_Door;
        CDestructibleIntelligence m_Destructible;
        CPickupIntelligence m_Pickup;
        CPlayerIntelligence m_Player;
    };
} CIntelligence; /* size 0x9C */

#endif
