#include "common.h"

/* Only what this touches: a result, a word cleared and the callback. */
typedef struct {
    /* 0x00 */ s32 result;
    /* 0x04 */ u8 pad4[0xC - 0x4];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ s32 (*callback)();
} Task;

void func_00468590(Task *arg0) {
    arg0->result = arg0->callback();
    arg0->unkC = 0;
}
