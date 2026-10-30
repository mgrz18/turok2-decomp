/* WRONG CUT: REAL func_0042C46C size 0x4C (splat cut it into func_0042C46C + func_0042C480). */
#include "common.h"

extern u8 D_800D1CD8[];
extern u8 D_800E6AC0[];

extern s32 func_0028D0E0(void);
extern void func_00201DBC(void *, s32, s32, void *);

typedef struct MenuState {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ s32 unk34;
} MenuState;

void func_0042C46C(MenuState *menu) {
    if (func_0028D0E0() == 0) {
        func_00201DBC(D_800D1CD8, menu->unk2C, menu->unk34, D_800E6AC0);
    }
}
