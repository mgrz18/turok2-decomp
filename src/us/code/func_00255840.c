#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA5C];
    /* 0xA5C */ f32 unkA5C;
    /* 0xA60 */ u8 padA60[0xA7C - 0xA60];
    /* 0xA7C */ f32 unkA7C;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 unk1C;
} Arg1;

extern f32 D_800A7188;
extern f32 D_800A718C;
extern f32 D_800A7190;

void func_00255840(Player *arg0, Arg1 *arg1) {
    if (arg0->unkA5C < 0.0f) {
        arg0->unkA7C = (f32) D_800A7188;
    } else {
        arg0->unkA7C = (f32) D_800A718C;
    }
    arg1->unk1C = (f32) D_800A7190;
}
