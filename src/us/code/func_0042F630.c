#include "common.h"

/* Something with a handler at 0x14 that this calls. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 (*handler)();
} HandlerHolder;

void func_0042F630(HandlerHolder *arg0) {
    arg0->handler();
}
