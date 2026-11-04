#include "common.h"

/* The state at *D_800C1BB0 (cleared by func_002837E0, see func_002838A4). */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 unk38;
} State;

extern State *D_800C1BB0;

s32 func_00284174(void) {
    return D_800C1BB0->unk38;
}
