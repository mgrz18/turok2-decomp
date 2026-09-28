#include "common.h"
#include "m2c_macros.h"

extern s32 func_00291034(s32, void *, s32);

typedef struct {
    s16 kind;
    s16 pad;
    s32 a;
    s32 b;
    s32 c;
} Msg;

void func_00290EF0(s32 arg0) {
    Msg m;

    m.kind = 0x11;
    func_00291034(arg0 + 0x48, &m, 0);
}
