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

void func_00288C5C(GameState *arg0, s32 arg1) {
    arg0->unk23FE1 = 2;
    arg0->unk23FDC = arg1;
}
