#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA08];
    /* 0xA08 */ u16 pose;
    /* 0xA0A */ u8 padA0A[0xA64 - 0xA0A];
    /* 0xA64 */ s32 heldButtons;
    /* 0xA68 */ s32 pressedButtons;
    /* 0xA6C */ u8 padA6C[0xAC0 - 0xA6C];
    /* 0xAC0 */ f32 crouch;
    /* 0xAC4 */ s32 crouchToggle;
} Player;

extern u8 D_8011AAD8[];   /* input state */
extern f32 D_800B6D28;    /* frame time scale */

/* Crouch: toggled by button 0x400 while standing or walking (poses 2 and 3),
 * forced by flags 0x6000; eases the crouch height toward 40.96 or 0 and sets
 * bit 0x80 of *out while it is within 10.24 of fully crouched. */
void func_00256604(Player *player, s32 unused, s32 *flags, s32 *out) {
    s32 crouching;
    f32 delta;
    u8 *input;

    crouching = 0;
    if (*flags & 0x6000) {
        crouching = 1;
    } else if ((u16)(player->pose - 2) < 2) {
        if (player->pressedButtons & 0x400) {
            input = D_8011AAD8;
            if (input[0x19] == 0) {
                if (*(s32 *)(input - 0x38) == 0 || !(player->heldButtons & 0x20)) {
                    player->crouchToggle ^= 1;
                }
            }
        }
    } else {
        player->crouchToggle = 0;
    }
    if (player->crouchToggle != 0) {
        crouching = 1;
    }
    if (crouching) {
        delta = 0.25f * (40.96f - player->crouch);
    } else {
        delta = -player->crouch * 0.25f;
    }
    player->crouch += delta * D_800B6D28;
    delta = 40.96f - player->crouch;
    if (-10.24f <= delta && delta <= 10.24f) {
        *out |= 0x80;
    } else {
        *out &= ~0x80;
    }
}
