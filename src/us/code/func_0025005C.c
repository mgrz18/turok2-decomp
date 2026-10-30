#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f *corners[3];
} Floor;

typedef struct {
    /* 0x00 */ u8 pad0[0xC6];
    /* 0xC6 */ s8 stickX;
    /* 0xC7 */ s8 stickY;
} Controller;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ Floor *floor;
    /* 0x014 */ u8 pad14[0x1C - 0x14];
    /* 0x01C */ f32 velY;
    /* 0x020 */ u8 pad20[0x50 - 0x20];
    /* 0x050 */ f32 yaw;
} Body;

typedef struct {
    /* 0x0000 */ u8 pad0[0x51C];
    /* 0x051C */ void *hud;
    /* 0x0520 */ u8 pad520[0xA08 - 0x520];
    /* 0x0A08 */ s16 state;
    /* 0x0A0A */ u8 padA0A[0xA50 - 0xA0A];
    /* 0x0A50 */ Controller *controller;
    /* 0x0A54 */ u8 padA54[0xA5C - 0xA54];
    /* 0x0A5C */ f32 strafeInput;
    /* 0x0A60 */ f32 forwardInput;
    /* 0x0A64 */ u32 held;
    /* 0x0A68 */ u32 pressed;
    /* 0x0A6C */ u8 padA6C[0xA78 - 0xA6C];
    /* 0x0A78 */ f32 forward;
    /* 0x0A7C */ f32 strafe;
    /* 0x0A80 */ u8 padA80[0xAA0 - 0xA80];
    /* 0x0AA0 */ Vec3f velocity;
    /* 0x0AAC */ u8 padAAC[0xAC8 - 0xAAC];
    /* 0x0AC8 */ f32 pitch;
    /* 0x0ACC */ u8 padACC[0xAD0 - 0xACC];
    /* 0x0AD0 */ f32 tilt;
} Player;

extern u8 D_800F7078[];
extern u8 D_801198B0[];
extern char D_800A6EA8[]; /* "Fly Mode Off" */
extern f32 D_800B6D28;    /* frame time */

void func_002117DC(f32 *value, f32 target, f32 rate);
f32 func_0021160C(f32 value, f32 target, f32 rate);
f32 func_0021170C(f32 value, f32 rate);
f32 func_002982F0(f32 x); /* sin */
f32 func_002974C0(f32 x); /* cos */
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);
Floor *func_00220A1C(void *world, Vec3f *pos);
s32 func_0024E5F0(Player *player, Body *body, s32 mode);
void func_0027FC2C(u8 *game, void *hud, char *msg);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* Fly mode (cheat): turn and pitch with the stick, fly forward/sideways and
 * up/down with the buttons; holding all three 0xE000 buttons and pressing one
 * again leaves it, dropping onto the middle of the floor under the player. */
void func_0025005C(Player *player, Body *body) {
    Vec3f move;
    Floor *floor;
    u32 held;

    if (player->state == 0 && !(player->held & 0xE000)) {
        player->state = 1;
    }
    player->tilt = 0.0f;
    func_002117DC(&body->yaw, body->yaw + player->controller->stickX * 0.0043633236f, 0.9f);
    func_002117DC(&player->pitch, player->pitch + -player->controller->stickY * 0.0021816618f, 0.9f);
    if (player->forwardInput != 0.0f) {
        player->forward = func_0021160C(player->forward, player->forwardInput * 8.533333f,
                                        ABS(player->forwardInput) * 102.399994f);
    } else {
        player->forward = func_0021170C(player->forward, 17.066666f);
    }
    if (player->strafeInput != 0.0f) {
        player->strafe = func_0021160C(player->strafe, player->strafeInput * 8.533333f,
                                       ABS(player->strafeInput) * 102.399994f);
    } else {
        player->strafe = func_0021170C(player->strafe, 17.066666f);
    }
    held = player->held;
    if (held & 0x10) {
        body->velY = func_0021160C(body->velY, 5.9733334f, 71.68f);
    } else if (held & 0x2020) {
        body->velY = func_0021160C(body->velY, -5.9733334f, 71.68f);
    } else {
        body->velY = func_0021170C(body->velY, 11.946667f);
    }
    move.x = player->forward * func_002982F0(body->yaw + 3.1415927f);
    move.y = body->velY;
    move.z = player->forward * func_002974C0(body->yaw + 3.1415927f);
    move.x += player->strafe * func_002982F0(body->yaw - 1.5707964f);
    move.z += player->strafe * func_002974C0(body->yaw - 1.5707964f);
    func_0020EF60(&move, &move, D_800B6D28);
    func_0020EEF8(&player->velocity, &player->velocity, &move);
    floor = func_00220A1C(D_800F7078, &body->pos);
    if (floor != NULL) {
        body->floor = floor;
    }
    if (player->state == 1 && (player->held & 0xE000) == 0xE000 && (player->pressed & 0xE000)) {
        func_0024E5F0(player, body, 2);
        func_0027FC2C(D_801198B0, player->hud, D_800A6EA8);
        if (floor == NULL) {
            floor = body->floor;
            body->pos = *floor->corners[0];
            func_0020EEF8(&body->pos, &body->pos, floor->corners[1]);
            func_0020EEF8(&body->pos, &body->pos, floor->corners[2]);
            func_0020EF60(&body->pos, &body->pos, 0.33333334f);
        }
    }
}
