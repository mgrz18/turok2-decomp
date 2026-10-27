/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_17.CPP, func_80224C4C_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x22];
    /* 0x22 */ s16 strokeSound;
} Tuning;

typedef struct {
    /* 0x000 */ s32 count;
    /* 0x004 */ f32 a;
    /* 0x008 */ f32 b;
    /* 0x00C */ u8 padC[0x37C - 0xC];
    /* 0x37C */ s32 last;
} Strokes;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} Surface;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ Surface *surface;
    /* 0x018 */ u8 pad18[4];
    /* 0x01C */ f32 climb;
    /* 0x020 */ u8 pad20[0x140 - 0x20];
    /* 0x140 */ s32 flags;
} Body;

typedef struct {
    /* 0x0000 */ u8 pad0[0xE2];
    /* 0x00E2 */ s8 idle;
    /* 0x00E3 */ u8 padE3[0xA08 - 0xE3];
    /* 0x0A08 */ s16 mode;
    /* 0x0A0A */ u8 padA0A[0xA10 - 0xA0A];
    /* 0x0A10 */ f32 stateTime;
    /* 0x0A14 */ u8 padA14[0xA60 - 0xA14];
    /* 0x0A60 */ f32 lift;
    /* 0x0A64 */ u32 buttons;
    /* 0x0A68 */ u32 pressed;
    /* 0x0A6C */ u8 padA6C[0xA78 - 0xA6C];
    /* 0x0A78 */ f32 forward;
    /* 0x0A7C */ f32 strafe;
    /* 0x0A80 */ f32 speed;
    /* 0x0A84 */ u8 padA84[0xA9C - 0xA84];
    /* 0x0A9C */ f32 depth;
    /* 0x0AA0 */ u8 padAA0[4];
    /* 0x0AA4 */ f32 height;
    /* 0x0AA8 */ u8 padAA8[0xAC8 - 0xAA8];
    /* 0x0AC8 */ f32 pitch;
    /* 0x0ACC */ f32 roll;
    /* 0x0AD0 */ f32 tilt;
    /* 0x0AD4 */ f32 sway[3];
    /* 0x0AE0 */ u8 padAE0[4];
    /* 0x0AE4 */ f32 eyeOffset;
    /* 0x0AE8 */ u8 padAE8[0xB8C - 0xAE8];
    /* 0x0B8C */ s32 zoomed;
    /* 0x0B90 */ u8 padB90[0xC04 - 0xB90];
    /* 0x0C04 */ s32 animation;
    /* 0x0C08 */ u8 padC08[0xCD0 - 0xC08];
    /* 0x0CD0 */ Strokes strokes;
    /* 0x1050 */ u8 pad1050[0x1290 - 0x1050];
    /* 0x1290 */ f32 soundTime;
} Player;

extern f32 D_800B6D28;      /* frame time */
extern char D_800ADE70;     /* standing look tuning */
extern Tuning D_800ADEB8;   /* wading move tuning */

void func_002117DC(f32 *value, f32 target, f32 rate);
void func_0024EF70(Player *player, Body *body, void *tuning);
void func_0024F174(Player *player, Body *body, void *tuning);
s32 func_0024E5F0(Player *player, Body *body, s32 mode);
f32 func_00268A5C(Body *body);
f32 func_002982F0(f32 x);
void func_00275544(s16 sound, Vec3f pos, s32, s32);

static __inline__ void reset_strokes(Strokes *s) {
    s->count = 0;
    s->a = 0.0f;
    s->last = -1;
    s->b = 0.0f;
}

static __inline__ s32 try_jump(Player *player, Body *body) {
    if (player->zoomed == 0 && !(11.25f < player->depth) && !(body->flags & 0x6000)
        && (body->surface->flags & 2) && (player->pressed & 0x10)) {
        func_0024E5F0(player, body, 4);
        return 1;
    }
    return 0;
}

/* Wading: leave the state once out of the water, dive when looking down hard
 * while lifting, else follow the water surface, sway, and pick the animation
 * from the climb; moving plays the tuning's stroke sound now and then. */
void func_002508D0(Player *player, Body *body) {
    s32 idle;

    if (player->speed == 0.0f) {
        player->mode = 8;
    } else {
        player->mode = 9;
    }
    func_002117DC(&player->tilt, 0.0f, 0.25f);
    func_0024EF70(player, body, &D_800ADE70);
    func_0024F174(player, body, &D_800ADEB8);
    if (!(body->flags & 0x200)) {
        reset_strokes(&player->strokes);
        func_0024E5F0(player, body, 3);
        return;
    }
    if (player->pitch * 57.295776f < -40.0f && player->lift > 0.0f) {
        player->height = func_00268A5C(body) - player->eyeOffset - 51.199997f;
        func_0024E5F0(player, body, 0xB);
        return;
    }
    if (!(body->flags & 0xC0000)) {
        body->climb = 0.0f;
        func_002117DC(&player->height, func_00268A5C(body) - player->eyeOffset * 0.6f, 0.5f);
    }
    player->sway[0] = func_002982F0(player->stateTime * 0.06981318f) * 0.043633237f;
    player->sway[1] = func_002982F0(player->stateTime * 0.087266475f) * 0.043633237f;
    player->sway[2] = func_002982F0(player->stateTime * 0.104719765f) * 0.043633237f;
    if (try_jump(player, body)) {
        return;
    }
    idle = 0;
    if (player->animation == 0x1144) {
        idle = player->idle == 0;
    }
    if (!idle) {
        if (1.024f <= player->forward) {
            player->animation = 0x8A2;
        } else if (player->forward <= -1.024f) {
            player->animation = 0x8A7;
        } else {
            player->animation = 0x14;
        }
    }
    player->soundTime += D_800B6D28;
    if (5.12f < player->speed && 11.25f < player->soundTime) {
        player->soundTime = 0.0f;
        func_00275544(D_800ADEB8.strokeSound, body->pos, 0, -1);
    }
}
