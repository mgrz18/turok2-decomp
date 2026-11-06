#include "common.h"

s32 COverlay__StartScript(void *, s32 *);           /* extern */
extern s32 D_800B2C64;

typedef struct {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ u8 sub48[0x84 - 0x48];
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 unk8C;
} Unk0027AD54;

void COnScreen__SetBoss2(Unk0027AD54 *arg0, s32 arg1, s32 arg2) {
    arg0->unk88 = arg1;
    arg0->unk84 = arg2;
    arg0->unk8C = 0x64;
    COverlay__StartScript(arg0->sub48, &D_800B2C64);
}
