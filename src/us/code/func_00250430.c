/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_15.CPP, func_80224408_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} Surface;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ Surface *surface;
    /* 0x018 */ u8 pad18[0x50 - 0x18];
    /* 0x050 */ f32 heading;
    /* 0x054 */ u8 pad54[0x140 - 0x54];
    /* 0x140 */ s32 flags;
} Body;

typedef struct {
    /* 0x000 */ u8 pad0[0xE2];
    /* 0x0E2 */ s8 idle;
    /* 0x0E3 */ u8 padE3[0xA5C - 0xE3];
    /* 0xA5C */ f32 strafe;
    /* 0xA60 */ f32 lift;
    /* 0xA64 */ u32 buttons;
    /* 0xA68 */ u32 pressed;
    /* 0xA6C */ u8 padA6C[0xA9C - 0xA6C];
    /* 0xA9C */ f32 depth;
    /* 0xAA0 */ u8 padAA0[0xAC0 - 0xAA0];
    /* 0xAC0 */ f32 crouch;
    /* 0xAC4 */ u8 padAC4[0xB8C - 0xAC4];
    /* 0xB8C */ s32 zoomed;
    /* 0xB90 */ u8 padB90[0xC04 - 0xB90];
    /* 0xC04 */ s32 animation;
} Player;

extern char D_800ADE70;     /* standing look tuning */
extern char D_800ADE88;     /* crouching look tuning */

void func_0024FC08(Player *player, Body *body);
void func_0024EF70(Player *player, Body *body, void *tuning);
s32 func_0024E5F0(Player *player, Body *body, s32 mode);

static __inline__ s32 try_jump(Player *player, Body *body) {
    if (player->zoomed == 0 && !(11.25f < player->depth) && !(body->flags & 0x6000)
        && (body->surface->flags & 2) && (player->pressed & 0x10)) {
        func_0024E5F0(player, body, 4);
        return 1;
    }
    return 0;
}

/* Standing and walking: bob the view, turn it with the crouching or standing
 * tuning, hand off to a jump or to moving, then pick the turning animation. */
void func_00250430(Player *player, Body *body) {
    f32 heading;
    s32 crouching;
    s32 animate;

    func_0024FC08(player, body);
    heading = body->heading;
    crouching = 5.12f < player->crouch;
    if (crouching) {
        func_0024EF70(player, body, &D_800ADE88);
    } else {
        func_0024EF70(player, body, &D_800ADE70);
    }
    heading = body->heading - heading;
    if (try_jump(player, body)) {
        return;
    }
    if (player->strafe != 0.0f || (player->zoomed != 2 && player->lift != 0.0f)) {
        func_0024E5F0(player, body, 3);
    }
    animate = 1;
    if ((player->animation == 0x1130 || player->animation == 0x1135) && player->idle == 0) {
        animate = 0;
    }
    if (animate) {
        if (body->flags & 0x6000) {
            player->animation = 0xA28;
        } else if (0.017453294f < heading) {
            player->animation = 0xB5E;
        } else if (heading < -0.017453294f) {
            player->animation = 0xB54;
        } else {
            player->animation = 1;
        }
    }
}
