#include "common.h"

typedef struct {
    /* 0x00 */ s32 a;
    /* 0x04 */ s32 b;
    /* 0x08 */ s32 c;
    /* 0x0C */ s32 d;
    /* 0x10 */ s32 e;
} Words5;

void func_00245828(Words5 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    arg0->a = arg1;
    arg0->b = arg2;
    arg0->c = arg3;
    arg0->d = arg4;
    arg0->e = arg5;
}
