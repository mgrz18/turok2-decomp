/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_12.CPP, func_802238E0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 x, y, z, w;
} Quat;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ f32 climb;
} Body;

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

typedef struct {
    /* 0x0000 */ u8 pad0[0x996];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xA08 - 0x998];
    /* 0x0A08 */ s16 mode;
    /* 0x0A0A */ u8 padA0A[0xA24 - 0xA0A];
    /* 0x0A24 */ f32 moveScale;
    /* 0x0A28 */ u8 padA28[0xA54 - 0xA28];
    /* 0x0A54 */ f32 turnInput;
    /* 0x0A58 */ u8 padA58[4];
    /* 0x0A5C */ f32 strafeInput;
    /* 0x0A60 */ f32 forwardInput;
    /* 0x0A64 */ u32 held;
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
    /* 0x0AAC */ u8 padAAC[0xAD0 - 0xAAC];
    /* 0x0AD0 */ f32 tilt;
} Player;

extern f32 D_800B6D28;      /* frame time */
extern Mode D_800ADF5C[];

f32 func_0021170C(f32 value, f32 rate);
f32 func_0021160C(f32 value, f32 delta, f32 limit);
f32 func_00298470(f32 x);
void func_002117DC(f32 *value, f32 target, f32 rate);
Quat func_0021AE6C(Body *body);
void func_00211114(Quat *q, f32 *mtx);
void func_0020F85C(f32 *mtx, Vec3f *in, Vec3f *out);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);
void func_00275544(s16 sound, Vec3f pos, s32, s32);

/* Swimming movement: forward and strafe speeds, the dodge impulses and the
 * climb, turned by the body's facing and added to the velocity, with drag
 * on the way down and a roll from the strafe. */
void func_0024F658(Player *player, Body *body, Tuning *tuning) {
    Vec3f forward;
    Vec3f strafe;
    Vec3f forwardOut;
    Vec3f strafeOut;
    Vec3f sum;
    f32 mtx[16];
    Quat rot;
    f32 value;
    f32 max;
    f32 speed;
    f32 climb;
    f32 climbMax;
    s32 ready;
    s32 impulse;
    u32 pressed;

    impulse = 0;
    value = player->impulseTime;
    ready = 1;
    if (!(value <= 0.0f)) {
        ready = 0;
        player->impulseTime = value - D_800B6D28;
    }
    if (player->weapon == 0x12 || player->weapon == 0x21) {
        ready = 0;
        if (player->held & 8) {
            player->forward = func_0021160C(player->forward, tuning->max * 0.75f, tuning->max * 3.0f);
        }
    }
    value = player->forward;
    max = tuning->max;
    if (value < 0.0f ? max < -value : max < value) {
        player->forward = func_0021170C(player->forward, tuning->decel * 0.5f);
    } else if (player->forwardInput != 0.0f) {
        player->forward = func_0021160C(player->forward, player->forwardInput * tuning->accel,
            (player->forwardInput < 0.0f ? -player->forwardInput : player->forwardInput) * tuning->max);
    } else {
        player->forward = func_0021170C(player->forward, tuning->decel);
    }
    value = player->strafe;
    max = tuning->max;
    if (value < 0.0f ? max < -value : max < value) {
        player->strafe = func_0021170C(player->strafe, tuning->decel * 0.5f);
    } else if (player->strafeInput != 0.0f) {
        player->strafe = func_0021160C(player->strafe, player->strafeInput * tuning->accel,
            (player->strafeInput < 0.0f ? -player->strafeInput : player->strafeInput) * tuning->max * 0.75f);
    } else {
        player->strafe = func_0021170C(player->strafe, tuning->decel);
    }
    if (ready) {
        pressed = player->pressed;
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
        speed = func_0021170C(player->forwardImpulse, tuning->impulseDecel);
    }
    player->forwardImpulse = speed;
    if (ready && (player->pressed2 & 1)) {
        player->strafeImpulse = tuning->impulse * 0.75f;
        impulse = 1;
    } else if (ready && (player->pressed2 & 2)) {
        player->strafeImpulse = -tuning->impulse * 0.75f;
        impulse = 1;
    } else {
        player->strafeImpulse = func_0021170C(player->strafeImpulse, tuning->impulseDecel * 0.75f);
    }
    if (ready && (player->pressed & 0x10)) {
        impulse = 1;
        body->climb += tuning->climbImpulse;
    }
    if (player->held & 0x10) {
        climb = body->climb;
        climbMax = tuning->climbMax;
        if (climb < climbMax) {
            body->climb = func_0021160C(climb, tuning->climbAccel, climbMax);
        }
    }
    forward.x = 0.0f;
    forward.y = 0;
    forward.z = player->forward + player->forwardImpulse;
    strafe.x = player->strafe + player->strafeImpulse;
    strafe.y = 0;
    strafe.z = 0;
    rot = func_0021AE6C(body);
    func_00211114(&rot, mtx);
    func_0020F85C(mtx, &forward, &forwardOut);
    func_0020F85C(mtx, &strafe, &strafeOut);
    player->velocity.x += (forwardOut.x + strafeOut.x) * player->moveScale;
    player->velocity.y += (forwardOut.y + strafeOut.y) * player->moveScale;
    player->velocity.z += (forwardOut.z + strafeOut.z) * player->moveScale;
    if (body->climb < 0.0f) {
        func_0020EEF8(&sum, &forwardOut, &strafeOut);
        body->climb -= D_800ADF5C[player->mode].physics->drag
            * (func_00298470(sum.x * sum.x + sum.y * sum.y + sum.z * sum.z) / (tuning->max * 4.0f))
            * player->moveScale;
    }
    func_002117DC(&player->tilt, (player->strafe * 1.8f + player->turnInput * 16.0f) * 0.017453294f, 0.04f);
    if (impulse) {
        player->impulseTime += 7.5f;
        func_00275544(tuning->impulseSound, body->pos, 0, -1);
    }
}
