#include "common.h"

s32 func_002245BC(s32 *, u16, u16, s32); /* extern */
extern s32 D_800F7078;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ u16 unk4;
    /* 0x6 */ u8 pad6[0xA - 0x6];
    /* 0xA */ u16 unkA;
} Unk00264900;

void func_00266540(Unk00264900 *arg0) {
    func_002245BC(&D_800F7078, arg0->unkA, arg0->unk4, 0);
}
