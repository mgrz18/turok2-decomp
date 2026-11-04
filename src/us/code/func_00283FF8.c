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

s32 func_00412438();                                /* extern */
extern State *D_800C1BB0;

s32 func_00283FF8(s32 arg0, s32 (*arg1)(void *), s32 (*arg2)()) {
    s32 temp_s3;

    temp_s3 = func_00412438();
    if (D_800C1BB0->scriptId == arg0) {
        D_800C1BB0->onStart = arg1;
        D_800C1BB0->onFinish = arg2;
    } else {
        if (arg1 != NULL) {
            arg1(D_800C1BB0);
        }
        if (arg2 != NULL) {
            arg2();
        }
    }
    return temp_s3;
}
