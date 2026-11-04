#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 (*callback)();
} Arg0;

void func_00468550(Arg0 *arg0) {
    arg0->callback();
}
