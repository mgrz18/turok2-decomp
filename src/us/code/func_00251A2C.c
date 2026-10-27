/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00225F20.c, func_00225F20, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 m[4][4];
} Mtx4f;

/* The ridden creature (an AI actor). */
typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[0x34];
    /* 0x174 */ s8 aiState;
} Mount;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0x50 - 0x10];
    /* 0x50 */ f32 yaw;
} Body;

typedef struct {
    /* 0x0000 */ u8 pad0[0x51C];
    /* 0x051C */ void *ui;
    /* 0x0520 */ u8 pad520[0xA54 - 0x520];
    /* 0x0A54 */ f32 stickX;
    /* 0x0A58 */ u8 padA58[0xA70 - 0xA58];
    /* 0x0A70 */ u32 released;
    /* 0x0A74 */ u8 padA74[4];
    /* 0x0A78 */ f32 forward;
    /* 0x0A7C */ f32 strafe;
    /* 0x0A80 */ u8 padA80[0xAAC - 0xA80];
    /* 0x0AAC */ f32 fov;
    /* 0x0AB0 */ u8 padAB0[0xAD0 - 0xAB0];
    /* 0x0AD0 */ f32 tilt;
    /* 0x0AD4 */ u8 padAD4[0xAE0 - 0xAD4];
    /* 0x0AE0 */ Vec3f viewOffset;
    /* 0x0AEC */ u8 padAEC[0xB14 - 0xAEC];
    /* 0x0B14 */ s16 queuedWeapon;
    /* 0x0B16 */ u8 padB16[0xBB0 - 0xB16];
    /* 0x0BB0 */ Mount *mount;
    /* 0x0BB4 */ s32 mountType;
    /* 0x0BB8 */ Vec3f seat;
    /* 0x0BC4 */ u8 padBC4[0xBDC - 0xBC4];
    /* 0x0BDC */ f32 swayTime;
    /* 0x0BE0 */ f32 stepDistance;
} Player;

extern f32 D_800B6D28;          /* frame time */
extern f32 D_800ADDE4;          /* charge speed */
extern f32 D_800AE344;          /* step length */
extern Vec3f D_800ADCB0;        /* rider view offset */
extern Vec3f D_800ADCBC;        /* rider seat */
extern char D_800ADEA0;         /* riding look tuning */
extern char D_800ADDE0;         /* riding forward move tuning */
extern char D_800ADE04;         /* riding backward move tuning */

f32 func_002974C0(f32 x);
void func_002117DC(f32 *value, f32 target, f32 rate);
void func_0024EF70(Player *player, Body *body, void *tuning);
void func_0024F174(Player *player, Body *body, void *tuning);
void func_00243414(Mount *actor, void *ai, s32 state);
void func_002811CC(void *ui, f32 a, f32 b, f32 c, f32 d, s32 e, Vec3f pos);
void func_002105A4(Mtx4f *m, f32 angle);
void func_0020F85C(Mtx4f *m, Vec3f *in, Vec3f *out);
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);

/* Riding: the mount sways the view as it turns, charges on button 8, shakes
 * the screen at each step and sets the rider's view offset. */
void func_00251A2C(Player *player, Body *body) {
    Mtx4f m;
    Mount *mount;
    Vec3f *offset;
    f32 turn;
    f32 yaw;
    f32 fwd;
    f32 t;
    f32 step;
    f32 dist;
    f32 next;
    f32 shake;

    mount = player->mount;
    turn = player->stickX * (((func_002974C0(player->swayTime * 0.2792527f) + 1.0f) * 0.01f + 0.06f) * 80.0f);
    player->swayTime += D_800B6D28;
    turn *= 0.017453294f;
    yaw = body->yaw;
    func_002117DC(&body->yaw, yaw + turn, 0.9f);
    func_002117DC(&player->tilt, ((yaw - body->yaw) + player->stickX * 8.0f) * 0.017453294f, 0.1f);
    func_0024EF70(player, body, &D_800ADEA0);

    offset = NULL;
    if (player->released & 8) {
        func_00243414(mount, mount->ai, 0x2E);
        player->fov = 90.0f;
        player->forward = D_800ADDE4 * 2;
    }

    fwd = player->forward;
    dist = player->stepDistance;
    if (fwd < 0.0f) {
        fwd = -fwd;
    }
    if ((player->strafe * 2 < 0.0f ? -(player->strafe * 2) : player->strafe * 2) < fwd) {
        step = player->forward;
        if (step < 0.0f) {
            next = dist - step;
        } else {
            next = dist + step;
        }
    } else {
        step = player->strafe * 2;
        if (step < 0.0f) {
            next = dist - step;
        } else {
            next = dist + step;
        }
    }
    player->stepDistance = next;
    if (D_800AE344 < next) {
        player->stepDistance = next - D_800AE344;
        if (mount->aiState != 0x2E) {
            player->forward *= 0.5f;
            player->strafe *= 0.5f;
        }
        t = player->forward;
        if (t < 0.0f) {
            shake = -t * 2.5f;
        } else {
            shake = t * 2.5f;
        }
        if (50.0f < shake) {
            shake = 50.0f;
        }
        func_002811CC(player->ui, 0.0f, shake, 0.0f, 1.0f, 1, body->pos);
    }

    if (player->forward > 0.0f) {
        func_0024F174(player, body, &D_800ADDE0);
    } else {
        func_0024F174(player, body, &D_800ADE04);
    }
    t = player->forward;
    if ((t < 0.0f ? -t : t) > 10.24f) {
        player->fov = 15.0f;
    }

    if (player->mountType == 0x136) {
        offset = &D_800ADCB0;
        player->seat = D_800ADCBC;
    }
    if (offset != NULL) {
        func_002105A4(&m, body->yaw);
        func_0020F85C(&m, offset, &player->viewOffset);
        func_0020EF60(&player->viewOffset, &player->viewOffset, 10.24f);
    }
    player->queuedWeapon = 0x13;
}
