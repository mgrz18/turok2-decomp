#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;

typedef struct { s16 type; s16 pad; s16 val; s16 pad2; s32 pad3[2]; } Msg;
extern void func_00291034(void *, Msg *, s32);

void func_00290EC0(u8 *arg0, s16 arg1) {
    Msg m;

    m.type = 0xA;
    m.val = arg1;
    func_00291034(arg0 + 0x48, &m, 0);
}
