#include "common.h"

s32 func_00287ABC(s32);
s32 func_0028D23C(void);

extern s32 D_800B6D14;
extern s32 D_800B6D24;
extern s32 D_800B6D7C;
extern s32 D_8011B110;

/* The frame/game state block passed to the per-mode handlers (the
 * handler table D_800B6D78/D_800B6D7C has 0xC-byte rows indexed by mode). */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FD0];
    /* 0x23FD0 */ s32 unk23FD0;
    /* 0x23FD4 */ s32 prevMode;
    /* 0x23FD8 */ s32 mode;
    /* 0x23FDC */ s32 nextMode;
    /* 0x23FE0 */ u8 pad23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
    /* 0x23FE2 */ u8 pad23FE2[0x23FE4 - 0x23FE2];
    /* 0x23FE4 */ f32 fadeTime;
    /* 0x23FE8 */ u8 pad23FE8[0x23FF0 - 0x23FE8];
    /* 0x23FF0 */ s32 unk23FF0;
} GameState;

void func_00288C80(GameState *arg0) {
    s32 (*temp_v0)(void *);

    if (D_800B6D14 != 0) {
        D_800B6D14 = 0;
    }
    func_0028D23C();
    temp_v0 = *(((__typeof__(&D_800B6D7C))((s8 *)&D_800B6D7C + (arg0->mode * 0xC))));
    if (temp_v0 != NULL) {
        temp_v0(arg0);
    }
    if ((arg0->mode != 0x11) && (D_8011B110 != 0)) {
        D_8011B110 -= 1;
        func_00287ABC((s32) arg0);
    }
    D_800B6D24 += 1;
}
