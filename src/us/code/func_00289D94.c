#include "common.h"

/* The game state (GameState in func_00286440). */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FDC];
    /* 0x23FDC */ s32 unk23FDC;
    /* 0x23FE0 */ u8 pad23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
    /* 0x23FE2 */ u8 pad23FE2[0x23FF0 - 0x23FE2];
    /* 0x23FF0 */ s32 unk23FF0;
} GameState;

void func_0029AA80(s32);
s32 func_0029AAD0(s32);

s32 func_0029AC40(f32);                         /* extern */
extern f32 D_800A9E24;
extern s32 D_800B7BE0;
extern s32 D_800B7E60;
extern s32 D_800C2038;

void func_00289D94(GameState *arg0) {
    s32 *var_a0;

    if (arg0->unk23FF0 != 0) {
        var_a0 = &D_800B7E60;
        if (D_800C2038 == 0) {
            var_a0 = &D_800B7BE0;
        }
        func_0029AA80((s32) var_a0);
        func_0029AC40(D_800A9E24);
        func_0029AAD0(2);
    }
}
