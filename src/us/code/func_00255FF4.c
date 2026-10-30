#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ f32 velY;
    /* 0x020 */ u8 pad20[0xA08 - 0x20];
    /* 0xA08 */ s16 state;
    /* 0xA0A */ u8 padA0A[0xA10 - 0xA0A];
    /* 0xA10 */ f32 unkA10;
    /* 0xA14 */ u8 padA14[0xA1C - 0xA14];
    /* 0xA1C */ u32 flagsA1C;
    /* 0xA20 */ u8 padA20[0xAD0 - 0xA20];
    /* 0xAD0 */ f32 unkAD0;
    /* 0xAD4 */ u8 padAD4[0xBF0 - 0xAD4];
    /* 0xBF0 */ s32 unkBF0;
} Player;

extern void func_002117DC(f32 *value, f32 target, f32 rate);
extern void func_0042E29C(Player *player);

void func_00255FF4(Player *player) {
    if (player->state == 29) {
        player->velY = -307.19998f;
    }
    if (player->unkBF0 == 0) {
        if (!(player->flagsA1C & 0x8000)) {
            func_002117DC(&player->unkAD0, 1.308997f, 0.25f);
        }
        if (player->unkA10 > 37.5f) {
            func_0042E29C(player);
        }
    }
}
