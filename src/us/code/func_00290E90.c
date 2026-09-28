#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;

typedef struct { s16 type; s32 val; s32 pad[2]; } Msg;
extern void func_00291034(void *, Msg *, s32);

void func_00290E90(u8 *arg0, s32 arg1) {
    Msg m;

    m.type = 0xD;
    m.val = arg1;
    func_00291034(arg0 + 0x48, &m, 0);
}
