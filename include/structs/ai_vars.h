#ifndef STRUCTS_AI_VARS_H
#define STRUCTS_AI_VARS_H

/* Views of CAI::m_VarBuffer (CAI + 0x10C, 0x30 bytes), one per AI type that
 * keeps its own state there: the union in structs/game_object.h has a member
 * for each (m_Enemy, m_Turret, m_Platform, m_Generator). Offsets below are
 * from the start of the buffer (CAI + 0x10C + offset).
 *
 * Angles: m_Pitch turns about X (CMtxF__RotateX / CMtxF__SetRotateX), m_Roll about
 * Z (CMtxF__SetRotateZ), both applied before the yaw. */

#include "common.h"
#include "structs/vector.h"

struct CGameObjectInstance;

/* Type 1 (CEnemyIntelligence), as the enemy AI uses it: EnemyAI_Init sets it
 * up. m_Behavior[] are the definition's behaviour for each mode
 * (CEnemyIntelligence::m_Behaviors[0], [1], m_UnderwaterBehavior): ground,
 * air, water; m_BehaviorIndex is the current one (EnemyAI_GetBehaviorIndex returns it).
 * The flying steering (EnemyFly_SteerToPath, EnemyFly_SteerToPathScaled, behaviour 1) pitches
 * with the vertical speed and rolls with the turn; the swimming modes
 * (EnemySwim_Setup sets them up) turn at m_TurnRate and pulse m_BaseScale.
 * Timers are in frames, counted down by gFrameIncrement. m_Timer and m_Timer2 run
 * one after the other (EnemyMode_CloakActivate_Enter seeds both); when both are out the AI
 * goes to mode 0x19, whose draw fades with m_Timer (EnemyAI_DrawFaded). */
typedef struct {
    /* 0x00 */ f32 m_Pitch;          /* eases towards -vertical speed (EnemyFly_SteerToPath, EnemySwim_Advance) */
    /* 0x04 */ f32 m_Roll;           /* eases towards the turn (EnemyFly_SteerToPath) */
    /* 0x08 */ struct CGameObjectInstance *m_pGrabbed; /* the other side of a grab: the actor it holds
                                                         * (EnemyMode_PickupThrowObject_Update lets it go) or, while held
                                                         * (m_ModeFlags 0x10000), the holder (EnemyMode_PlayerRide_Update,
                                                         * Projectile_PlaceAtMuzzle) */
    /* 0x0C */ f32 m_TurnRate;       /* swimming: passed to AI_TurnClamped */
    /* 0x10 */ f32 m_BaseScale;      /* 1.0 (EnemyAI_Init), the scale (EnemySwim_Setup) */
    /* 0x14 */ f32 m_AttackDelay;    /* no projectile/weapon attack until it is out */
    /* 0x18 */ s32 unk18;          /* zeroed by EnemyAI_Init */
    /* 0x1C */ f32 m_WanderTimer;    /* swimming: re-picks m_vTargetOffset when out (EnemySwim_Advance) */
    /* 0x20 */ f32 m_IdleTimer;      /* when out, switch between the ground and air behaviours
                                      * (EnemyAI_IdleSoundCallback); a flier chasing a target in the other one
                                      * cuts it to 0-15 frames (EnemyFly_SteerToPath) */
    /* 0x24 */ s8 m_Behavior[3];
    /* 0x27 */ s8 m_BehaviorIndex;
    /* 0x28 */ f32 m_Timer;
    /* 0x2C */ f32 m_Timer2;
} CEnemyAIVars; /* size 0x30 */

/* Type 4 (CTurretIntelligence): the gun's pitch, eased towards the sight
 * target's (Turret_Aim); CGameObjectInstance__GetRotation builds the turret's facing from it. */
typedef struct {
    /* 0x00 */ f32 m_Pitch;
} CTurretAIVars;

/* Type 2 (CPlatformIntelligence). The "special" sink (Platform_UpdateSpecial): once
 * m_SpecialTimer reaches m_SpecialDelay the platform shakes (m_vSpecialOffset
 * random, 45 frames), then sinks (m_vSpecialOffset.y towards
 * -m_SpecialSinkDist over m_SpecialSinkTime). m_Pitch/m_Roll tilt its
 * matrix (Platform_CalcOrientation) and collision box (Platform_GetCollisionBox). */
typedef struct {
    /* 0x00 */ f32 m_SpecialTimer;
    /* 0x04 */ CVector3 m_vSpecialOffset;
    /* 0x10 */ f32 m_Pitch;
    /* 0x14 */ f32 m_Roll;
} CPlatformAIVars;

/* Type 3 (CGeneratorIntelligence): what it has spawned (Generator_SpawnEnter). */
typedef struct {
    /* 0x00 */ s32 m_nActive;        /* the spawned objects' owner counter (passed by address) */
    /* 0x04 */ s32 m_nRemaining;     /* one less per spawn */
} CGeneratorAIVars;

#endif /* STRUCTS_AI_VARS_H */
