#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 flags;
} Request;

void func_00204D9C(Request *arg0) {
    arg0->count = (s32) (arg0->count + 1);
    arg0->flags = (s32) (arg0->flags | 0x100);
}
