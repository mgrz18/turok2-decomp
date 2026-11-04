#include "common.h"

/* The font colour COnScreen__SetFontColor sets (D_80110190): six words, in argument order. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} FontColor;

extern FontColor D_80110190;

void COnScreen__SetFontColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    D_80110190.unk0 = arg0;
    D_80110190.unk4 = arg1;
    D_80110190.unk8 = arg2;
    D_80110190.unkC = arg3;
    D_80110190.unk10 = arg4;
    D_80110190.unk14 = arg5;
}
