#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ s32 unkB0;
} Arg;

extern Arg *D_800C1BB0;

s32 func_002843F4(void) {
    return D_800C1BB0->unkB0;
}
