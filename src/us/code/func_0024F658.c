/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_12.CPP, func_802238E0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ f32 accel;
    /* 0x04 */ f32 max;
    /* 0x08 */ f32 decel;
    /* 0x0C */ f32 impulse;
    /* 0x10 */ f32 impulseDecel;
    /* 0x14 */ f32 climbAccel;
    /* 0x18 */ f32 climbMax;
    /* 0x1C */ f32 climbImpulse;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 impulseSound;
} Tuning;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ f32 drag;
} Physics;

typedef struct {
    /* 0x00 */ Physics *physics;
    /* 0x04 */ u8 pad4[0x18 - 4];
} Mode;

extern f32 gFrameIncrement;      /* frame time */
extern Mode D_800ADF5C[];

f32 Math_DecayToZero(f32 value, f32 rate);
f32 Math_StepClamped(f32 value, f32 delta, f32 limit);
f32 func_00298470(f32 x);
void Math_EaseToward(f32 *value, f32 target, f32 rate);
CQuaternion CGameObjectInstance__GetRotation(CGameObjectInstance *body);
void CQuaternion__ToMatrix(CQuaternion *q, f32 *mtx);
void CMtxF__TransformPoint(f32 *mtx, CVector3 *in, CVector3 *out);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
void Sound__Play(s16 sound, CVector3 pos, s32, s32);

/* Swimming movement: forward and strafe speeds, the dodge impulses and the
 * climb, turned by the body's facing and added to the velocity, with drag
 * on the way down and a roll from the strafe. */
void CPlayer__MoveInWater(CPlayer *player, CGameObjectInstance *body, Tuning *tuning) {
    CVector3 forward;
    CVector3 strafe;
    CVector3 forwardOut;
    CVector3 strafeOut;
    CVector3 sum;
    f32 mtx[16];
    CQuaternion rot;
    f32 value;
    f32 max;
    f32 speed;
    f32 climb;
    f32 climbMax;
    s32 ready;
    s32 impulse;
    u32 pressed;

    impulse = 0;
    value = player->m_BurstTimer;
    ready = 1;
    if (!(value <= 0.0f)) {
        ready = 0;
        player->m_BurstTimer = value - gFrameIncrement;
    }
    if (player->m_Data.m_CurrentWeapon == 0x12 || player->m_Data.m_CurrentWeapon == 0x21) {
        ready = 0;
        if (player->m_ContState.m_dwButtons & 8) {
            player->m_Speed = Math_StepClamped(player->m_Speed, tuning->max * 0.75f, tuning->max * 3.0f);
        }
    }
    value = player->m_Speed;
    max = tuning->max;
    if (value < 0.0f ? max < -value : max < value) {
        player->m_Speed = Math_DecayToZero(player->m_Speed, tuning->decel * 0.5f);
    } else if (player->m_ContState.m_FinalMoveY != 0.0f) {
        player->m_Speed = Math_StepClamped(player->m_Speed, player->m_ContState.m_FinalMoveY * tuning->accel,
            (player->m_ContState.m_FinalMoveY < 0.0f ? -player->m_ContState.m_FinalMoveY : player->m_ContState.m_FinalMoveY) * tuning->max);
    } else {
        player->m_Speed = Math_DecayToZero(player->m_Speed, tuning->decel);
    }
    value = player->m_SideSpeed;
    max = tuning->max;
    if (value < 0.0f ? max < -value : max < value) {
        player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, tuning->decel * 0.5f);
    } else if (player->m_ContState.m_FinalMoveX != 0.0f) {
        player->m_SideSpeed = Math_StepClamped(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * tuning->accel,
            (player->m_ContState.m_FinalMoveX < 0.0f ? -player->m_ContState.m_FinalMoveX : player->m_ContState.m_FinalMoveX) * tuning->max * 0.75f);
    } else {
        player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, tuning->decel);
    }
    if (ready) {
        pressed = player->m_ContState.m_dwButtonsSingle;
        if (pressed & 8) {
            speed = tuning->impulse;
            impulse = 1;
        } else if (pressed & 4) {
            speed = -tuning->impulse * 0.5f;
            impulse = 1;
        } else {
            goto decay;
        }
    } else {
    decay:
        speed = Math_DecayToZero(player->m_BurstSpeed, tuning->impulseDecel);
    }
    player->m_BurstSpeed = speed;
    if (ready && (player->m_ContState.m_dwButtonsDouble & 1)) {
        player->m_BurstSideSpeed = tuning->impulse * 0.75f;
        impulse = 1;
    } else if (ready && (player->m_ContState.m_dwButtonsDouble & 2)) {
        player->m_BurstSideSpeed = -tuning->impulse * 0.75f;
        impulse = 1;
    } else {
        player->m_BurstSideSpeed = Math_DecayToZero(player->m_BurstSideSpeed, tuning->impulseDecel * 0.75f);
    }
    if (ready && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        impulse = 1;
        body->m_vVelocity.y += tuning->climbImpulse;
    }
    if (player->m_ContState.m_dwButtons & 0x10) {
        climb = body->m_vVelocity.y;
        climbMax = tuning->climbMax;
        if (climb < climbMax) {
            body->m_vVelocity.y = Math_StepClamped(climb, tuning->climbAccel, climbMax);
        }
    }
    forward.x = 0.0f;
    forward.y = 0;
    forward.z = player->m_Speed + player->m_BurstSpeed;
    strafe.x = player->m_SideSpeed + player->m_BurstSideSpeed;
    strafe.y = 0;
    strafe.z = 0;
    rot = CGameObjectInstance__GetRotation(body);
    CQuaternion__ToMatrix(&rot, mtx);
    CMtxF__TransformPoint(mtx, &forward, &forwardOut);
    CMtxF__TransformPoint(mtx, &strafe, &strafeOut);
    player->m_vDesiredPos.x += (forwardOut.x + strafeOut.x) * player->m_FrameIncrementScaled;
    player->m_vDesiredPos.y += (forwardOut.y + strafeOut.y) * player->m_FrameIncrementScaled;
    player->m_vDesiredPos.z += (forwardOut.z + strafeOut.z) * player->m_FrameIncrementScaled;
    if (body->m_vVelocity.y < 0.0f) {
        CVector3__Add(&sum, &forwardOut, &strafeOut);
        body->m_vVelocity.y -= D_800ADF5C[player->m_Mode].physics->drag
            * (func_00298470(sum.x * sum.x + sum.y * sum.y + sum.z * sum.z) / (tuning->max * 4.0f))
            * player->m_FrameIncrementScaled;
    }
    Math_EaseToward(&player->m_vHeadRotOffset.z, (player->m_SideSpeed * 1.8f + player->m_ContState.m_FinalLookX * 16.0f) * 0.017453294f, 0.04f);
    if (impulse) {
        player->m_BurstTimer += 7.5f;
        Sound__Play(tuning->impulseSound, body->header.m_vPos, 0, -1);
    }
}
