#include "common.h"

/* Two HUD meters (func_0027AD24 / func_0027AD54 start them): the arena timer and kill count of func_004082A0. */
typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 active;
    /* 0x44 */ s32 percent;
} HudMeter; /* size 0x48 */

typedef struct {
    /* 0x00 */ HudMeter meters[2];
} HudMeters;

typedef struct { f32 x, y, z; } V3f;

extern u8 D_800B2C50[];
extern void func_0027AD00(void *, void *);

void func_0027AD24(HudMeters *arg0, s32 arg1, s32 arg2) {
    arg0->meters[0].active = arg1;
    arg0->meters[0].unk3C = arg2;
    arg0->meters[0].percent = 100;
    func_0027AD00(arg0, D_800B2C50);
}
