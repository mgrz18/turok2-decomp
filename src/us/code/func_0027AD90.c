#include "common.h"

/* Two HUD meters (COnScreen__SetBoss / COnScreen__SetBoss2 start them): the arena timer and kill count of TotemDefense_UpdateFight. */
typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 active;
    /* 0x44 */ s32 percent;
} HudMeter; /* size 0x48 */

typedef struct {
    /* 0x00 */ HudMeter meters[2];
} HudMeters;

void COnScreen__SetBossPercent2(HudMeters *arg0, s32 arg1) {
    arg0->meters[1].percent = arg1;
}
