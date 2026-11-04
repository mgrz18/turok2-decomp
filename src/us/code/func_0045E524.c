#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 slots[3];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Unk0045E524;

void func_0045E524(Unk0045E524 *arg0) {
    s32 var_v1;
    s32 *var_v0;

    var_v1 = 2;
    var_v0 = &arg0->unk8;   /* var_v0[3] walks slots[2..0] */
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unk18 = 0;
    arg0->unk1C = 0;
    do {
        var_v0[3] = 0;
        var_v1 -= 1;
        var_v0 -= 1;
    } while (var_v1 >= 0);
}
