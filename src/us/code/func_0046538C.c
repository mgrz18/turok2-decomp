#include "common.h"

s32 func_00201DBC(s32, s32, s32, s32);

s32 func_0028D0E0();                                /* extern */
extern s32 D_800D1CD8;
extern s32 D_800E6AC0;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ s32 unk34;
} Unk0046538C;

void func_0046538C(Unk0046538C *arg0) {
    if (func_0028D0E0() == 0) {
        func_00201DBC((s32) &D_800D1CD8, arg0->unk2C, arg0->unk34, (s32) &D_800E6AC0);
    }
}
