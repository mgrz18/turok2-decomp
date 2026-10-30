#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad0[0x4C - 0x8];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ u8 pad1[0x88 - 0x50];
} Info00403144;

typedef struct {
    /* 0x00 */ u8 pad0[0xC7];
    /* 0xC7 */ s8 count;
} Item00403144;

extern s32 D_8012F5F4;
void func_00246108(s32 a0, Item00403144 *item, Info00403144 *info, s32 a3, s32 a4);
void func_00245BAC(s32 a0, Item00403144 *item, f32 a2, s32 a3);
s32 func_002451AC(s32 a0, Item00403144 *item, s32 a2, f32 a3);
void func_00243414(s32 a0, Item00403144 *item, s32 a2);

static __inline__ s32 func_00403774_inline(s32 a0, Item00403144 *item, s32 a2) {
    s32 result;

    if (D_8012F5F4 == 0) {
        result = 0;
    } else {
        result = func_002451AC(a0, item, a2, 0.785398245f);
    }
    return result;
}

void func_00403144(s32 a0, Item00403144 *item) {
    Info00403144 info;
    s32 side;

    func_00246108(a0, item, &info, 1, 1);
    func_00245BAC(a0, item, 0.17453295f, info.unk4C);
    side = func_00403774_inline(a0, item, info.unk4);
    if (side == -1) {
        func_00243414(a0, item, 11);
    } else if (side == 1) {
        func_00243414(a0, item, 12);
    } else if (item->count >= 4) {
        func_00243414(a0, item, 8);
    }
}
