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

void func_0027ADA0(HudMeters *arg0) {
    arg0->meters[1].active = 0;
}
