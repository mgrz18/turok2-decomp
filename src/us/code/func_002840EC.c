#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ s32 active;
    /* 0x64 */ f32 level;
} Arg;

s32 func_00284144();                                /* extern */
s32 func_00284174();                                /* extern */
extern Arg *D_800C1BB0;

void func_002840EC(void) {
    if ((func_00284174() != 0) && (func_00284144() != 0) && (D_800C1BB0->active == 0)) {
        D_800C1BB0->active = 1;
        D_800C1BB0->level = 1.0f;
    }
}
