#include "common.h"

s32 func_00416644(s32, s32, s32, s32, s32);

s32 func_00421250();                            /* extern */
extern s32 D_0043B6F0;
extern s32 D_80119E2C;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Unk00419618;

s32 func_00419618(s32 arg0, Unk00419618 *arg1) {
    func_00421250();
    func_00416644((s32) &D_80119E2C, (s32) &D_0043B6F0, arg1->unk1C, arg1->unk20, 0);
    return 1;
}
