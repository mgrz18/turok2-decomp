#include "common.h"

/* The sun: only what CSun__GetSunData hands out. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[0x20 - 0x14];
    /* 0x20 */ s32 unk20;
} CSun;

void CSun__GetSunData(CSun *arg0, s32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4) {
    *arg1 = arg0->unk20;
    *arg4 = arg0->unk10;
    *arg2 = arg0->unk8;
    *arg3 = arg0->unkC;
}
