#include "common.h"

typedef struct {
    /* 0x00 */ s32 result;
    /* 0x04 */ u8 pad4[0xC - 0x4];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ s32 (*func)();
} Job;

void func_00468570(Job *arg0) {
    arg0->func();
}
