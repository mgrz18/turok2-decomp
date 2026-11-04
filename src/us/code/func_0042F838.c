#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 countdown;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 (*callback)();
} Unk0042F838;

void func_0042F838(Unk0042F838 *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->countdown - 1;
    arg0->countdown = temp_v0;
    if (temp_v0 <= 0) {
        arg0->callback();
    }
}
