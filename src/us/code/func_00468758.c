#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 counter;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 (*callback)();
} Timer;

/* Counts down; calls the callback when it runs out. */
void func_00468758(Timer *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->counter - 1;
    arg0->counter = temp_v0;
    if (temp_v0 <= 0) {
        arg0->callback();
    }
}
