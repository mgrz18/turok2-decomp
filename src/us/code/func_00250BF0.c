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
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Vec3f pos;
} Ui;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x1C - 0x10];
    /* 0x01C */ f32 climb;
    /* 0x020 */ u8 pad20[0x140 - 0x20];
    /* 0x140 */ s32 flags;
} Body;

typedef struct {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ u8 pad10[0xE2 - 0x10];
    /* 0x00E2 */ s8 idle;
    /* 0x00E3 */ u8 padE3[0x51C - 0xE3];
    /* 0x051C */ Ui *ui;
    /* 0x0520 */ u8 pad520[0xA08 - 0x520];
    /* 0x0A08 */ s16 mode;
    /* 0x0A0A */ u8 padA0A[0xA10 - 0xA0A];
    /* 0x0A10 */ f32 stateTime;
    /* 0x0A14 */ u8 padA14[0xA78 - 0xA14];
    /* 0x0A78 */ f32 forward;
    /* 0x0A7C */ f32 strafe;
    /* 0x0A80 */ f32 speed;
    /* 0x0A84 */ u8 padA84[0xAD4 - 0xA84];
    /* 0x0AD4 */ f32 sway[3];
    /* 0x0AE0 */ u8 padAE0[0xBE4 - 0xAE0];
    /* 0x0BE4 */ f32 air;
    /* 0x0BE8 */ u8 padBE8[4];
    /* 0x0BEC */ s32 drowning;
    /* 0x0BF0 */ u8 padBF0[0xC04 - 0xBF0];
    /* 0x0C04 */ s32 animation;
    /* 0x0C08 */ u8 padC08[0xCD0 - 0xC08];
    /* 0x0CD0 */ Strokes strokes;
    /* 0x1050 */ u8 pad1050[0x1290 - 0x1050];
    /* 0x1290 */ f32 soundTime;
} Player;

extern f32 D_800B6D28;      /* frame time */
typedef struct {
    u8 multiplayer;
} GameMode;

extern GameMode D_8011AAF1;
extern char D_800ADF00;     /* swimming look tuning */
extern Tuning D_800ADEDC;   /* swimming move tuning */

void func_0024EF70(Player *player, Body *body, void *tuning);
void func_0024F658(Player *player, Body *body, void *tuning);
s32 func_0024E5F0(Player *player, Body *body, s32 mode);
f32 func_002982F0(f32 x);
f32 func_0021149C(f32 scale);
void func_00275544(s16 sound, Vec3f pos, void *source, s32);

static __inline__ void reset_strokes(Strokes *s) {
    s->count = 0;
    s->a = 0.0f;
    s->last = -1;
    s->b = 0.0f;
}

/* Swimming: sway, move with the swimming tuning, and on surfacing (after two
 * time units) gasp for air and leave the state for falling, wading or
 * walking; then pick the animation from the climb and play strokes. */
void func_00250BF0(Player *player, Body *body) {
    s32 idle;

    if (player->speed == 0.0f) {
        player->mode = 0xA;
    } else {
        player->mode = 0xB;
    }
    func_0024EF70(player, body, &D_800ADF00);
    player->sway[0] = func_002982F0(player->stateTime * 0.06981318f) * 2 * 0.017453294f;
    player->sway[1] = func_002982F0(player->stateTime * 0.087266475f) * 2 * 0.017453294f;
    player->sway[2] = func_002982F0(player->stateTime * 0.104719765f) * 2 * 0.017453294f;
    func_0024F658(player, body, &D_800ADEDC);
    if (!(body->flags & 0x80) && 2.0f < player->stateTime) {
        reset_strokes(&player->strokes);
        body->climb = 0.0f;
        if (!D_8011AAF1.multiplayer) {
            if (player->drowning) {
                func_00275544(0x3AC3, player->pos, &player->pos, -1);
            } else if (52.5f < player->air) {
                func_00275544(0x3AC2, player->ui->pos, &player->ui->pos, -1);
            }
        }
        if (body->flags & 0xC0000) {
            func_0024E5F0(player, body, 2);
        } else if (body->flags & 0x200) {
            func_0024E5F0(player, body, 9);
        } else {
            func_0024E5F0(player, body, 3);
        }
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
    if (5.12f < player->speed && 45.0f < player->soundTime) {
        player->soundTime = func_0021149C(15.0f);
        func_00275544(D_800ADEDC.strokeSound, body->pos, NULL, -1);
    }
}
