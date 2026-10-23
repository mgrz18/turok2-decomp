#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 pad20[0x28 - 0x20];
    /* 0x28 */ s32 unk_28;
} State8011ACB0;

extern f32 D_800B6D28; /* the frame's time step */
extern State8011ACB0 D_8011ACB0;

extern void func_0024E5F0(void *player, void *other, s32 mode);

void func_00255F04(CPlayer *player) {
    State8011ACB0 *state;
    f32 timer;

    timer = P_F32(player, 0xBFC) + D_800B6D28;
    P_S32(player, 0xC04) = 1;
    P_S32(player, 0x18) = 0;
    P_S32(player, 0x1C) = 0;
    P_S32(player, 0x20) = 0;
    P_S32(player, 0x1298) = 0;
    P_S32(player, 0xD4) &= ~0x800000;
    P_F32(player, 0xBFC) = timer;
    if (timer > 75.0f || ((state = &D_8011ACB0)->unk_28 != 0 && state->unk_1C != 0)) {
        state = &D_8011ACB0;
        if (state->unk_28 != 0 && state->unk_1C == 0) {
            func_0024E5F0(player, player, 0x11);
        } else {
            func_0024E5F0(player, player, 0x10);
        }
    }
}
