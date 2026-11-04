#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xA0];
    /* 0xA0 */ f32 unkA0;
} Arg;

extern Arg *D_800C1BB0;

f32 func_002843D4(void) {
    return D_800C1BB0->unkA0 * 10.24f;
}
