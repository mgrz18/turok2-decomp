#include "common.h"

s32 func_0020367C(s32, s32, s32, s32, s32, s32 *, s32 *, s32); /* extern */
extern s32 func_00206990;
extern s32 D_800A51F0;

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} Unk0020798C;

void func_0020798C(Unk0020798C *arg0) {
    func_0020367C(0, arg0->unk0, arg0->unk4, 0, 0, &func_00206990, &D_800A51F0, 0);
}
