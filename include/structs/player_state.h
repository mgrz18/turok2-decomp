#ifndef STRUCTS_PLAYER_STATE_H
#define STRUCTS_PLAYER_STATE_H

/* The player's state table (gPlayerStateTable, one row per CPlayer m_Mode) and
 * the move / look tunings the state updates pass to CPlayer__MoveOnFoot,
 * CPlayer__MoveInWater and CPlayer__TurnAndLook (globals/player.h). */

#include "common.h"

/* CPlayer__MoveOnFoot's tuning (CPlayer__MoveInWater reads 0x0C-0x1C as the
 * impulse, its decay and the climb accel / max / impulse instead). */
typedef struct CMoveTuning {
    /* 0x00 */ f32 forwardAccel;
    /* 0x04 */ f32 forwardMax;
    /* 0x08 */ f32 forwardDecel;
    /* 0x0C */ f32 strafeAccel;
    /* 0x10 */ f32 strafeMax;
    /* 0x14 */ f32 strafeDecel;
    /* 0x18 */ f32 impulse;
    /* 0x1C */ f32 impulseDecel;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 impulseSound;            /* the stroke sound wading / swimming */
} CMoveTuning; /* size 0x24 */

/* CPlayer__TurnAndLook's tuning. */
typedef struct CLookTuning {
    /* 0x00 */ f32 turn;
    /* 0x04 */ f32 turnRate;
    /* 0x08 */ f32 lookRate;
    /* 0x0C */ f32 lookRateScale;
    /* 0x10 */ f32 lookDown;
    /* 0x14 */ f32 lookUp;
} CLookTuning; /* size 0x18 */

/* What a state row's physics points at (D_800F1E30, D_800F1F10). */
typedef struct PlayerStatePhysics {
    /* 0x00 */ u32 flags;                   /* tested for 0x80 by AI_FindBestTarget */
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ f32 drag;                    /* CPlayer__MoveInWater */
} PlayerStatePhysics;

/* A row of gPlayerStateTable (CPlayer__SetState, CPlayer__Update). */
typedef struct PlayerStateInfo {
    /* 0x00 */ void (*enter)(void *player, void *arg);
    /* 0x04 */ void (*update)(void *player, void *body);
    /* 0x08 */ PlayerStatePhysics *physics;
    /* 0x0C */ u32 flags;                   /* -> CPlayer m_ModeFlags */
    /* 0x10 */ s32 anim;                    /* -> CPlayer m_CharAnim when not 0 */
    /* 0x14 */ s16 pose;                    /* the weapon's pose (WeaponActor_Update, WeaponMode_*_Update) */
    /* 0x16 */ u8 pad16[2];
} PlayerStateInfo; /* size 0x18 */

/* {mask, bits}: the button remap of a control side (CPlayer__ReadInput). */
typedef struct ButtonMap {
    /* 0x00 */ u32 mask;
    /* 0x04 */ u32 bits;
} ButtonMap;

#endif
