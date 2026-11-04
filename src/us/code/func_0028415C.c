#include "common.h"

/* The state at *D_800C1BB0 (cleared by func_002837E0, see func_002838A4). */
typedef struct {
    /* 0x00 */ u8 pad0[0xDC];
    /* 0xDC */ s32 unkDC; /* -1 when unset */
} State;

extern State *D_800C1BB0;

s32 func_0028415C(void) {
    return ~D_800C1BB0->unkDC != 0;
}
