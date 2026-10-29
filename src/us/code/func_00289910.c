#include "common.h"

typedef struct {
    /* 0x00000 */ u8 pad0[0x3C4];
    /* 0x003C4 */ s32 unk3C4;
    /* 0x003C8 */ u8 pad3C8[0x23DF0 - 0x3C8];
    /* 0x23DF0 */ u8 unk23DF0[0x23E42 - 0x23DF0];
    /* 0x23E42 */ u8 unk23E42;
    /* 0x23E43 */ u8 pad23E43[0x23FD4 - 0x23E43];
    /* 0x23FD4 */ s32 mode;
    /* 0x23FD8 */ u8 pad23FD8[0x23FF8 - 0x23FD8];
    /* 0x23FF8 */ s32 unk23FF8;
    /* 0x23FFC */ s32 unk23FFC;
} GameState;

extern s32 D_8011ACF4;
extern s32 D_800B6D20;
extern f32 D_800B6D28[2];   /* frame time scales */
extern f32 D_800B6D30;
extern f32 D_800B6D34;
extern u8 D_800F7078[];
extern s32 D_801308F8;

extern void func_002886AC(void);
extern void func_0042F590(void);
extern s32 func_0028D0E0(void);
extern s32 func_0028D0D0(void);
extern void func_002888BC(GameState *, s32, s32);
extern void func_0028D100(void);
extern void CCollisionInfo__Reset(void);
extern void func_00430500(void *);
extern void func_0042C3BC(void *, s32);
extern void func_0042C700(void *, s32, s32);

void func_00289910(GameState *state) {
    s32 flag;

    D_8011ACF4 = 0;
    D_800B6D20 = 0;
    func_002886AC();
    flag = 1;
    func_0042F590();
    if (state->mode == 11 || state->mode == 12 || state->unk23FFC != 0) {
        flag = 0;
        if (state->unk23FFC != 0) {
            state->unk23FFC = 0;
        } else {
            state->unk23FF8 = 1000;
        }
    }
    if (func_0028D0E0()) {
        flag = 0;
        state->unk23FF8 = func_0028D0D0();
    }
    func_002888BC(state, state->unk23E42, flag);
    if (func_0028D0E0()) {
        func_0028D100();
    }
    CCollisionInfo__Reset();
    D_800B6D34 = 1.0f;
    D_800B6D28[1] = 1.0f;
    D_800B6D30 = 1.0f;
    func_00430500(state->unk23DF0);
    func_0042C3BC(D_800F7078, state->unk3C4);
    func_0042C700(D_800F7078, state->unk23FF8, 0);
    D_801308F8 = 0;
}
