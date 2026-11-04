/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_11.CPP, func_802233F0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[0x50 - 0x10];
    /* 0x50 */ f32 yaw;
} Body;

typedef struct {
    /* 0x00 */ f32 forwardAccel;
    /* 0x04 */ f32 forwardMax;
    /* 0x08 */ f32 forwardDecel;
    /* 0x0C */ f32 strafeAccel;
    /* 0x10 */ f32 strafeMax;
    /* 0x14 */ f32 strafeDecel;
    /* 0x18 */ f32 impulse;
    /* 0x1C */ f32 impulseDecel;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 impulseSound;
} Tuning;

extern f32 D_800B6D28;      /* frame time */

s32 func_00284174(void);
f32 func_0021170C(f32 value, f32 rate);
f32 func_0021160C(f32 value, f32 delta, f32 limit);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_0020F70C(CVector3 *v, f32 max);
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);
void func_00275544(s16 sound, CVector3 pos, s32, s32);

/* Walking movement: forward and strafe speeds towards the stick, the dodge
 * impulses, turned into world space and added to the velocity. */
void func_0024F174(CPlayer *player, Body *body, Tuning *tuning) {
    CVector3 motion;
    f32 forward;
    f32 strafe;
    f32 max;
    f32 time;
    s32 impulse;
    s32 ready;

    if (func_00284174() == 0) {
        impulse = 0;
        time = player->m_BurstTimer;
        ready = 1;
        if (!(time <= 0.0f)) {
            ready = 0;
            player->m_BurstTimer = time - D_800B6D28;
        }
        forward = player->m_Speed;
        time = tuning->forwardMax;
        if (forward < 0.0f ? time < -forward : time < forward) {
            player->m_Speed = func_0021170C(player->m_Speed, tuning->forwardDecel * 0.5f);
        } else if (player->m_ContState.m_FinalMoveY != 0.0f && player->m_SniperMode != 2) {
            player->m_Speed = func_0021160C(player->m_Speed, player->m_ContState.m_FinalMoveY * tuning->forwardAccel,
                (player->m_ContState.m_FinalMoveY < 0.0f ? -player->m_ContState.m_FinalMoveY : player->m_ContState.m_FinalMoveY) * tuning->forwardMax);
        } else {
            player->m_Speed = func_0021170C(player->m_Speed, tuning->forwardDecel);
        }
        motion.x = player->m_Speed * func_002982F0(body->yaw + 3.1415927f);
        motion.y = 0;
        motion.z = player->m_Speed * func_002974C0(body->yaw + 3.1415927f);
        strafe = player->m_SideSpeed;
        max = tuning->strafeMax;
        if (strafe < 0.0f ? max < -strafe : max < strafe) {
            player->m_SideSpeed = func_0021170C(player->m_SideSpeed, tuning->strafeDecel * 0.5f);
        } else if (player->m_ContState.m_FinalMoveX != 0.0f) {
            player->m_SideSpeed = func_0021160C(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * tuning->strafeAccel,
                (player->m_ContState.m_FinalMoveX < 0.0f ? -player->m_ContState.m_FinalMoveX : player->m_ContState.m_FinalMoveX) * tuning->strafeMax);
        } else {
            player->m_SideSpeed = func_0021170C(player->m_SideSpeed, tuning->strafeDecel);
        }
        motion.x += player->m_SideSpeed * func_002982F0(body->yaw - 1.5707964f);
        motion.z += player->m_SideSpeed * func_002974C0(body->yaw - 1.5707964f);
        strafe = player->m_SideSpeed;
        if (strafe < 0.0f) {
            strafe = -strafe;
        }
        forward = player->m_Speed;
        if (forward < 0.0f ? -forward < strafe : forward < strafe) {
            func_0020F70C(&motion, tuning->strafeMax);
        } else {
            func_0020F70C(&motion, tuning->forwardMax);
        }
        if (ready && (player->m_ContState.m_dwButtonsSingle & 8)) {
            player->m_BurstSpeed = tuning->impulse;
            impulse = 1;
        } else if (ready && (player->m_ContState.m_dwButtonsSingle & 4)) {
            player->m_BurstSpeed = -tuning->impulse * 0.5f;
            impulse = 1;
        } else {
            player->m_BurstSpeed = func_0021170C(player->m_BurstSpeed, tuning->impulseDecel);
        }
        motion.x += player->m_BurstSpeed * func_002982F0(body->yaw + 3.1415927f);
        motion.z += player->m_BurstSpeed * func_002974C0(body->yaw + 3.1415927f);
        if (ready && (player->m_ContState.m_dwButtonsDouble & 1)) {
            player->m_BurstSideSpeed = tuning->impulse * 0.75f;
            impulse = 1;
        } else if (ready && (player->m_ContState.m_dwButtonsDouble & 2)) {
            player->m_BurstSideSpeed = -tuning->impulse * 0.75f;
            impulse = 1;
        } else {
            player->m_BurstSideSpeed = func_0021170C(player->m_BurstSideSpeed, tuning->impulseDecel * 0.75f);
        }
        motion.x += player->m_BurstSideSpeed * func_002982F0(body->yaw - 1.5707964f);
        motion.z += player->m_BurstSideSpeed * func_002974C0(body->yaw - 1.5707964f);
        func_0020EF60(&motion, &motion, player->m_FrameIncrementScaled);
        func_0020EEF8(&player->m_vDesiredPos, &player->m_vDesiredPos, &motion);
        if (impulse) {
            player->m_BurstTimer += 7.5f;
            func_00275544(tuning->impulseSound, body->pos, 0, -1);
        }
    }
}
