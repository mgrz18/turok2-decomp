#ifndef STRUCTS_AI_VARS_BOSS_H
#define STRUCTS_AI_VARS_BOSS_H

/* Views of CAI::m_VarBuffer (structs/game_object.h, CAI at 0x10C) for the
 * bosses and the player's weapon actor: each AI type keeps its own state
 * there. The names come from the per-file CAI views these files had before
 * the CAI merge, and from what the code does with each word. */

#include "common.h"

/* The Blind One (D_80130200, structs/boss.h) and its three groups of level
 * actors (types 0x454, 0x455, 0x456): func_0040D5C4 sets up the boss,
 * func_0040D6A0 / func_0040D7A8 / func_0040D8B0 / func_0040DE68 the actors
 * (second copies func_004465C4...). */
typedef struct {
    /* 0x00 */ s32 m_nPhaseHits;  /* times it went down in this phase (func_0040E30C, which wakes
                                   * the next 0x456 actor only while that one's is 0); cleared by
                                   * every script step (func_0040E46C...) */
    /* 0x04 */ s32 m_nGroupIndex; /* the actor's index in its group (func_0040CCC0) */
    /* 0x08 */ s32 m_nFader;      /* its hit-flash animator, BlindOneState.faders[m_nFader] */
    /* 0x0C */ f32 m_HitTimer;    /* frames left of the hit flash (2.25 on a hit, counted down) */
    /* 0x10 */ f32 m_SightDist;   /* the flat distance to its sight target (func_0040E108) */
} CBlindOneAIVars; /* size 0x14 */

/* The Mother boss (D_8012FB70, structs/boss.h): its 0x1DB1 parts and 0x1DB2
 * controller get m_bActive (func_00409340); the module actors
 * (MotherBossState.actors) keep two scales that func_0040A25C animates with
 * the boss's anim state and applies to their parts. */
typedef struct {
    /* 0x00 */ u8 _pad0[0x4];
    /* 0x04 */ union {
        s32 m_bActive;             /* the parts and the controller */
        f32 m_PulseScale;          /* the actors: up to 3 and back to 1 in anim state 0x13 (parts 89-109) */
    };
    /* 0x08 */ f32 m_GrowScale;   /* the actors: up to 1 in anim state 9 (parts 21-61) */
} CMotherAIVars; /* size 0xC */

/* The player's first-person weapon actor (func_00257EA0-func_00259E30):
 * the barrel's spin, the fire cooldown and the model's draw arguments.
 * Switching weapons (func_00258668...) clears the barrel, the cooldown and
 * the level of detail. */
typedef struct {
    /* 0x00 */ f32 m_BarrelAngle;  /* turned by m_BarrelSpeed every frame (func_00259944, func_002593D8) */
    /* 0x04 */ f32 m_BarrelSpeed;
    /* 0x08 */ u8 _pad8[0xC - 0x8];
    /* 0x0C */ f32 m_FireDelay;    /* no fire until it is out: fireDelay of the WeaponInfo (func_00258C20) */
    /* 0x10 */ u8 _pad10[0x14 - 0x10];
    /* 0x14 */ f32 m_SpinTime;     /* func_00259944; cleared in mode 0 (func_00258000) */
    /* 0x18 */ u8 _pad18[0x1C - 0x18];
    /* 0x1C */ s32 m_nLod;         /* the model's level of detail (func_0025883C, func_0020B6A0) */
    /* 0x20 */ s32 m_nPass;        /* its draw pass, weapon 21 only */
    /* 0x24 */ f32 m_RefireDelay;  /* weapon 23 only: set when it fires (func_00259E30) */
} CWeaponAIVars; /* size 0x28 */

#endif
