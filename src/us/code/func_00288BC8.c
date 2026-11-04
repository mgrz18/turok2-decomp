#include "common.h"

s32 func_0028D0E0();                                /* extern */
extern f32 D_800A9DE0;

/* The frame/game state block passed to the per-mode handlers (the
 * handler table D_800B6D78/D_800B6D7C has 0xC-byte rows indexed by mode). */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FD0];
    /* 0x23FD0 */ s32 unk23FD0;
    /* 0x23FD4 */ s32 prevMode;
    /* 0x23FD8 */ s32 mode;
    /* 0x23FDC */ s32 nextMode;
    /* 0x23FE0 */ u8 pad23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
    /* 0x23FE2 */ u8 pad23FE2[0x23FE4 - 0x23FE2];
    /* 0x23FE4 */ f32 fadeTime;
    /* 0x23FE8 */ u8 pad23FE8[0x23FF0 - 0x23FE8];
    /* 0x23FF0 */ s32 unk23FF0;
} GameState;

void func_00288BC8(GameState *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = arg0->mode;
    arg0->unk23FE1 = 1;
    arg0->fadeTime = 0.0f;
    arg0->mode = 0x11;
    arg0->nextMode = arg1;
    arg0->prevMode = temp_v1;
    if (func_0028D0E0() != 0) {
        arg0->fadeTime = D_800A9DE0;
    }
}
