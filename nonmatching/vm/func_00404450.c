/* NON-MATCHING: 7 of 75 words differ: the loop addresses bonus/total from a register holding &mask (sym+0x3C4) in the ROM, from &bonus here; logic and the rest match. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3_00404450;

typedef struct {
    /* 0x000 */ s32 mode;
    /* 0x004 */ s32 total;
    /* 0x008 */ s32 bonus;
    /* 0x00C */ u8 pad0[0x380 - 0xC];
    /* 0x380 */ s32 values[2][8];
    /* 0x3C0 */ s32 unk3C0;
    /* 0x3C4 */ s32 mask;
    /* 0x3C8 */ u8 unk3C8;
    /* 0x3C9 */ u8 done;
    /* 0x3CA */ u8 pad1[0x3D0 - 0x3CA];
    /* 0x3D0 */ s32 unk3D0;
    /* 0x3D4 */ s32 unk3D4;
    /* 0x3D8 */ u8 unk3D8;
    /* 0x3D9 */ u8 unk3D9;
    /* 0x3DA */ u8 pad2[0x428 - 0x3DA];
    /* 0x428 */ Vec3_00404450 pos;
    /* 0x434 */ Vec3_00404450 startPos;
} ModeState00404450;

extern ModeState00404450 D_8012F608;
void func_00243414(s32 a0, s32 a1, s32 a2);
void func_00403C14(s32 mode);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

void func_00404450(s32 a0, s32 a1) {
    s32 count;
    s32 i;
    s32 mask;

    count = 0;
    D_8012F608.bonus = 0;
    D_8012F608.total = 0;
    mask = D_8012F608.mask;
    for (i = 0; i < 8; i++) {
        if (hasBit(mask, 1 << i)) {
            D_8012F608.bonus += D_8012F608.values[1][i];
            if (D_8012F608.values[0][i] > 0) {
                D_8012F608.total += D_8012F608.values[0][i];
                count++;
            }
        }
    }
    if (D_8012F608.done == 0) {
        if (D_8012F608.mode == 3) {
            if (count == 0) {
                func_00243414(a0, a1, 0x1D);
            }
        } else if (count == 0) {
            D_8012F608.mode++;
            if (D_8012F608.mode >= 2) {
                D_8012F608.unk3D4 = 3;
                D_8012F608.unk3D0 = 3;
                D_8012F608.unk3D8 = 0;
                D_8012F608.unk3D9 = 0;
                D_8012F608.pos.x = D_8012F608.startPos.x;
                D_8012F608.pos.y = D_8012F608.startPos.y;
                D_8012F608.pos.z = D_8012F608.startPos.z;
                func_00243414(a0, a1, 0x12);
            }
            func_00403C14(D_8012F608.mode);
        }
    }
}
