/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_16.CPP, func_8022470C_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} Surface;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ Surface *surface;
    /* 0x018 */ u8 pad18[0x140 - 0x18];
    /* 0x140 */ s32 flags;
} Body;

typedef struct {
    /* 0x0000 */ u8 pad0[0xA68];
    /* 0x0A68 */ u32 pressed;
    /* 0x0A6C */ u8 padA6C[0xA78 - 0xA6C];
    /* 0x0A78 */ f32 forward;
    /* 0x0A7C */ f32 strafe;
    /* 0x0A80 */ f32 speed;
    /* 0x0A84 */ u8 padA84[4];
    /* 0x0A88 */ s32 onGround;
    /* 0x0A8C */ u8 padA8C[0xA9C - 0xA8C];
    /* 0x0A9C */ f32 depth;
    /* 0x0AA0 */ u8 padAA0[0xAC0 - 0xAA0];
    /* 0x0AC0 */ f32 crouch;
    /* 0x0AC4 */ u8 padAC4[0xAD0 - 0xAC4];
    /* 0x0AD0 */ f32 tilt;
    /* 0x0AD4 */ u8 padAD4[0xB8C - 0xAD4];
    /* 0x0B8C */ s32 zoomed;
    /* 0x0B90 */ u8 padB90[0xC04 - 0xB90];
    /* 0x0C04 */ s32 animation;
    /* 0x0C08 */ u8 padC08[0x1290 - 0xC08];
    /* 0x1290 */ f32 soundTime;
} Player;

extern f32 D_800B6D28;      /* frame time */
extern char D_800ADE70;     /* standing look tuning */
extern char D_800ADE88;     /* crouching look tuning */
extern char D_800ADDBC;     /* standing move tuning */
extern char D_800ADE4C;     /* crouching move tuning */

void func_002117DC(f32 *value, f32 target, f32 rate);
void func_0024FE10(Player *player, Body *body);
void func_0024EF70(Player *player, Body *body, void *tuning);
void func_0024F174(Player *player, Body *body, void *tuning);
s32 func_0024E5F0(Player *player, Body *body, s32 mode);
void func_00275544(s16 sound, Vec3f pos, s32, s32);

static __inline__ s32 try_jump(Player *player, Body *body) {
    if (player->zoomed == 0 && !(11.25f < player->depth) && !(body->flags & 0x6000)
        && (body->surface->flags & 2) && (player->pressed & 0x10)) {
        func_0024E5F0(player, body, 4);
        return 1;
    }
    return 0;
}

/* Walking: bob the view, turn and move with the crouching or standing
 * tunings, hand off to a jump or to standing, play the wading sound in water,
 * then pick the walking animation from the direction. */
void func_002505F8(Player *player, Body *body) {
    f32 forward;
    f32 strafe;
    f32 time;
    s32 crouching;

    func_0024FE10(player, body);
    func_002117DC(&player->tilt, 0.0f, 0.25f);
    crouching = 5.12f < player->crouch;
    if (crouching) {
        func_0024EF70(player, body, &D_800ADE88);
    } else {
        func_0024EF70(player, body, &D_800ADE70);
    }
    crouching = 5.12f < player->crouch;
    if (crouching) {
        func_0024F174(player, body, &D_800ADE4C);
    } else {
        func_0024F174(player, body, &D_800ADDBC);
    }
    if (try_jump(player, body)) {
        return;
    }
    if (player->forward == 0.0f && player->strafe == 0.0f) {
        func_0024E5F0(player, body, 2);
    }
    if ((body->flags & 0x400) && player->onGround != 0) {
        time = player->soundTime + D_800B6D28;
        player->soundTime = time;
        if (player->speed > 5.12f && time >= 6.0f) {
            player->soundTime = time - 6.0f;
            func_00275544(0x3AB, body->pos, 0, -1);
        }
    }
    if (body->flags & 0x6000) {
        if (player->forward >= 0.0f) {
            player->animation = 0xA32;
        } else {
            player->animation = 0xA37;
        }
        return;
    }
    forward = player->forward;
    if (forward < 0.0f) {
        forward = -forward;
    }
    strafe = player->strafe;
    if (forward < (strafe < 0.0f ? -strafe : strafe)) {
        if (player->strafe < 0.0f) {
            player->animation = 0x802;
        } else {
            player->animation = 0x80C;
        }
    } else {
        if (player->forward >= 0.0f) {
            player->animation = 0x7DA;
        } else {
            player->animation = 0x7DF;
        }
    }
}
