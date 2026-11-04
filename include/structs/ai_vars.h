#ifndef STRUCTS_AI_VARS_H
#define STRUCTS_AI_VARS_H

/* Views of CAI::m_VarBuffer (CAI + 0x10C, 0x30 bytes), one per AI type that
 * keeps its own state there: the union in structs/game_object.h has a member
 * for each (m_Enemy, m_Turret, m_Platform, m_Generator). Offsets below are
 * from the start of the buffer (CAI + 0x10C + offset).
 *
 * Angles: m_Pitch turns about X (func_00210790 / func_00210518), m_Roll about
 * Z (func_00210630), both applied before the yaw. */

#include "common.h"
#include "structs/vector.h"

struct CGameObjectInstance;

/* Type 1 (CEnemyIntelligence), as the enemy AI uses it: func_0025BC70 sets it
 * up. m_Behavior[] are the definition's behaviour for each mode
 * (CEnemyIntelligence::m_Behaviors[0], [1], m_UnderwaterBehavior): ground,
 * air, water; m_BehaviorIndex is the current one (func_0025E428 returns it).
 * The flying steering (func_00260730, func_00260D60, behaviour 1) pitches
 * with the vertical speed and rolls with the turn; the swimming modes
 * (func_00261C38 sets them up) turn at m_TurnRate and pulse m_BaseScale.
 * Timers are in frames, counted down by D_800B6D28. m_Timer and m_Timer2 run
 * one after the other (func_0025F624 seeds both); when both are out the AI
 * goes to mode 0x19, whose draw fades with m_Timer (func_0025BA80). */
typedef struct {
    /* 0x00 */ f32 m_Pitch;          /* eases towards -vertical speed (func_00260730, func_00261490) */
    /* 0x04 */ f32 m_Roll;           /* eases towards the turn (func_00260730) */
    /* 0x08 */ struct CGameObjectInstance *m_pGrabbed; /* the other side of a grab: the actor it holds
                                                         * (func_0025F3A4 lets it go) or, while held
                                                         * (m_ModeFlags 0x10000), the holder (func_0025FE18,
                                                         * func_0025FEB0) */
    /* 0x0C */ f32 m_TurnRate;       /* swimming: passed to func_00245BAC */
    /* 0x10 */ f32 m_BaseScale;      /* 1.0 (func_0025BC70), the scale (func_00261C38) */
    /* 0x14 */ f32 m_AttackDelay;    /* no projectile/weapon attack until it is out */
    /* 0x18 */ s32 unk18;          /* zeroed by func_0025BC70 */
    /* 0x1C */ f32 m_WanderTimer;    /* swimming: re-picks m_vTargetOffset when out (func_00261490) */
    /* 0x20 */ f32 m_IdleTimer;      /* when out, switch between the ground and air behaviours
                                      * (func_0025C1B0); a flier chasing a target in the other one
                                      * cuts it to 0-15 frames (func_00260730) */
    /* 0x24 */ s8 m_Behavior[3];
    /* 0x27 */ s8 m_BehaviorIndex;
    /* 0x28 */ f32 m_Timer;
    /* 0x2C */ f32 m_Timer2;
} CEnemyAIVars; /* size 0x30 */

/* Type 4 (CTurretIntelligence): the gun's pitch, eased towards the sight
 * target's (func_002623EC); func_0021AE6C builds the turret's facing from it. */
typedef struct {
    /* 0x00 */ f32 m_Pitch;
} CTurretAIVars;

/* Type 2 (CPlatformIntelligence). The "special" sink (func_00263A20): once
 * m_SpecialTimer reaches m_SpecialDelay the platform shakes (m_vSpecialOffset
 * random, 45 frames), then sinks (m_vSpecialOffset.y towards
 * -m_SpecialSinkDist over m_SpecialSinkTime). m_Pitch/m_Roll tilt its
 * matrix (func_0026426C) and collision box (func_00241960). */
typedef struct {
    /* 0x00 */ f32 m_SpecialTimer;
    /* 0x04 */ CVector3 m_vSpecialOffset;
    /* 0x10 */ f32 m_Pitch;
    /* 0x14 */ f32 m_Roll;
} CPlatformAIVars;

/* Type 3 (CGeneratorIntelligence): what it has spawned (func_00262D74). */
typedef struct {
    /* 0x00 */ s32 m_nActive;        /* the spawned objects' owner counter (passed by address) */
    /* 0x04 */ s32 m_nRemaining;     /* one less per spawn */
} CGeneratorAIVars;

#endif /* STRUCTS_AI_VARS_H */
