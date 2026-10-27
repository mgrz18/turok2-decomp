/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SOUND/8022C894.CPP, func_8022CC34_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 vy;
} Body;

typedef struct {
    /* 0x000 */ u8 pad0[0xA18];
    /* 0xA18 */ s32 flags;
    /* 0xA1C */ u8 padA1C[0xAD0 - 0xA1C];
    /* 0xAD0 */ f32 lean;
    /* 0xAD4 */ u8 padAD4[0xC04 - 0xAD4];
    /* 0xC04 */ s32 animation;
} Player;

extern char D_800ADE70;
extern char D_800ADDBC;
extern void func_002117DC(f32 *value, f32 target, f32 rate);
extern void func_0024EF70(void *, void *, void *);
extern void func_0024F174(void *, void *, void *);
extern s32 func_0024E5F0(void *, void *, s32);
extern s32 func_002685F0(void *body);
extern f32 func_00267648(void *body, s32 arg);

/* Eases the lean back to zero, runs the movement tables, and on the ground
 * (falling) picks animation 0x7F8 when the slope speed is under 61.44. */
void func_00255714(Player *player, Body *body) {
    func_002117DC(&player->lean, 0.0f, 0.25f);
    func_0024EF70(player, body, &D_800ADE70);
    if (!(player->flags & 0x8000)) {
        func_0024F174(player, body, &D_800ADDBC);
    }
    if (body->vy <= 0.0f) {
        if (func_002685F0(body) != 0) {
            func_0024E5F0(player, body, 2);
        }
    }
    if (body->vy <= 0.0f) {
        if (func_00267648(body, 0) < 0.0f) {
            if (-func_00267648(body, 0) < 61.44f) {
                goto set_value;
            }
        } else if (func_00267648(body, 0) < 61.44f) {
set_value:
            player->animation = 0x7F8;
        }
    }
}
