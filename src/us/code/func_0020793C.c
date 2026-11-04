#include "common.h"

s32 func_0020367C(s32, s32, s32, s32, s32, s32 *, s32 *, s32); /* extern */
extern s32 func_00206990;
extern s32 D_800A51F0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
} Pair;

void func_0020793C(Pair *arg0) {
    func_0020367C(0, arg0->unk0, arg0->unk4, 0, 0, &func_00206990, &D_800A51F0, 1);
}
