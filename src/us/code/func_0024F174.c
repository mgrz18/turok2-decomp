/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_11.CPP, func_802233F0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
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

typedef struct {
    /* 0x0000 */ u8 pad0[0xA24];
    /* 0x0A24 */ f32 moveScale;
    /* 0x0A28 */ u8 padA28[0xA5C - 0xA28];
    /* 0x0A5C */ f32 strafeInput;
    /* 0x0A60 */ f32 forwardInput;
    /* 0x0A64 */ u8 padA64[4];
    /* 0x0A68 */ u32 pressed;
    /* 0x0A6C */ u8 padA6C[4];
    /* 0x0A70 */ u32 pressed2;
    /* 0x0A74 */ u8 padA74[4];
    /* 0x0A78 */ f32 forward;
    /* 0x0A7C */ f32 strafe;
    /* 0x0A80 */ u8 padA80[0xA8C - 0xA80];
    /* 0x0A8C */ f32 forwardImpulse;
    /* 0x0A90 */ f32 strafeImpulse;
    /* 0x0A94 */ f32 impulseTime;
    /* 0x0A98 */ u8 padA98[0xAA0 - 0xA98];
    /* 0x0AA0 */ Vec3f velocity;
    /* 0x0AAC */ u8 padAAC[0xB8C - 0xAAC];
    /* 0x0B8C */ s32 zoomed;
} Player;

extern f32 D_800B6D28;      /* frame time */

s32 func_00284174(void);
f32 func_0021170C(f32 value, f32 rate);
f32 func_0021160C(f32 value, f32 delta, f32 limit);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_0020F70C(Vec3f *v, f32 max);
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);
void func_00275544(s16 sound, Vec3f pos, s32, s32);

/* Walking movement: forward and strafe speeds towards the stick, the dodge
 * impulses, turned into world space and added to the velocity. */
void func_0024F174(Player *player, Body *body, Tuning *tuning) {
    Vec3f motion;
    f32 forward;
    f32 strafe;
    f32 max;
    f32 time;
    s32 impulse;
    s32 ready;

    if (func_00284174() == 0) {
        impulse = 0;
        time = player->impulseTime;
        ready = 1;
        if (!(time <= 0.0f)) {
            ready = 0;
            player->impulseTime = time - D_800B6D28;
        }
        forward = player->forward;
        time = tuning->forwardMax;
        if (forward < 0.0f ? time < -forward : time < forward) {
            player->forward = func_0021170C(player->forward, tuning->forwardDecel * 0.5f);
        } else if (player->forwardInput != 0.0f && player->zoomed != 2) {
            player->forward = func_0021160C(player->forward, player->forwardInput * tuning->forwardAccel,
                (player->forwardInput < 0.0f ? -player->forwardInput : player->forwardInput) * tuning->forwardMax);
        } else {
            player->forward = func_0021170C(player->forward, tuning->forwardDecel);
        }
        motion.x = player->forward * func_002982F0(body->yaw + 3.1415927f);
        motion.y = 0;
        motion.z = player->forward * func_002974C0(body->yaw + 3.1415927f);
        strafe = player->strafe;
        max = tuning->strafeMax;
        if (strafe < 0.0f ? max < -strafe : max < strafe) {
            player->strafe = func_0021170C(player->strafe, tuning->strafeDecel * 0.5f);
        } else if (player->strafeInput != 0.0f) {
            player->strafe = func_0021160C(player->strafe, player->strafeInput * tuning->strafeAccel,
                (player->strafeInput < 0.0f ? -player->strafeInput : player->strafeInput) * tuning->strafeMax);
        } else {
            player->strafe = func_0021170C(player->strafe, tuning->strafeDecel);
        }
        motion.x += player->strafe * func_002982F0(body->yaw - 1.5707964f);
        motion.z += player->strafe * func_002974C0(body->yaw - 1.5707964f);
        strafe = player->strafe;
        if (strafe < 0.0f) {
            strafe = -strafe;
        }
        forward = player->forward;
        if (forward < 0.0f ? -forward < strafe : forward < strafe) {
            func_0020F70C(&motion, tuning->strafeMax);
        } else {
            func_0020F70C(&motion, tuning->forwardMax);
        }
        if (ready && (player->pressed & 8)) {
            player->forwardImpulse = tuning->impulse;
            impulse = 1;
        } else if (ready && (player->pressed & 4)) {
            player->forwardImpulse = -tuning->impulse * 0.5f;
            impulse = 1;
        } else {
            player->forwardImpulse = func_0021170C(player->forwardImpulse, tuning->impulseDecel);
        }
        motion.x += player->forwardImpulse * func_002982F0(body->yaw + 3.1415927f);
        motion.z += player->forwardImpulse * func_002974C0(body->yaw + 3.1415927f);
        if (ready && (player->pressed2 & 1)) {
            player->strafeImpulse = tuning->impulse * 0.75f;
            impulse = 1;
        } else if (ready && (player->pressed2 & 2)) {
            player->strafeImpulse = -tuning->impulse * 0.75f;
            impulse = 1;
        } else {
            player->strafeImpulse = func_0021170C(player->strafeImpulse, tuning->impulseDecel * 0.75f);
        }
        motion.x += player->strafeImpulse * func_002982F0(body->yaw - 1.5707964f);
        motion.z += player->strafeImpulse * func_002974C0(body->yaw - 1.5707964f);
        func_0020EF60(&motion, &motion, player->moveScale);
        func_0020EEF8(&player->velocity, &player->velocity, &motion);
        if (impulse) {
            player->impulseTime += 7.5f;
            func_00275544(tuning->impulseSound, body->pos, 0, -1);
        }
    }
}
