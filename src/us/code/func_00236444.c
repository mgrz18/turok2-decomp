#include "common.h"

/* What this fills in on the element func_0026D3BC hands out. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} Slot;

Slot *func_0026D3BC(s32);

void func_00236444(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    Slot *temp_v0;

    temp_v0 = func_0026D3BC(arg0 + 0x7580);
    if (temp_v0 != 0) {
        temp_v0->unk18 = arg1;
        temp_v0->unk14 = arg2;
        temp_v0->unk8 = 0;
        temp_v0->unkC = arg4;
        temp_v0->unk10 = arg3;
    }
}
