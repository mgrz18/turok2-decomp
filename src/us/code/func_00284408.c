#include "common.h"

/* The state at *D_800C1BB0: a pending script with its start and finish callbacks. */
typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0xE0 - 0x40];
    /* 0xE0 */ s32 scriptId;
    /* 0xE4 */ s32 (*onStart)(void *);
    /* 0xE8 */ s32 (*onFinish)();
} State;

extern State *D_800C1BB0;

s32 func_00284408(void) {
    return D_800C1BB0->scriptId;
}
