#include "common.h"

/* The state at *D_800C1BB0. */
typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0xE0 - 0x40];
    /* 0xE0 */ s32 scriptId;
    /* 0xE4 */ s32 (*onStart)(void *);
    /* 0xE8 */ s32 (*onFinish)();
} State;

extern State *D_800C1BB0;

void func_002842E8(void) {
    D_800C1BB0->unk3C = 0;
}
