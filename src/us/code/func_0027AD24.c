#include "common.h"

/* COnScreen's two boss bars (COnScreen__SetBoss / COnScreen__SetBoss2 start
 * them): m_pBoss, m_pBossName and m_BossPercent, and the second boss's
 * (docs/HUD.md). Not an arena timer and kill count, as this comment once
 * said. */
typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 unk3C;   /* m_pBossName */
    /* 0x40 */ s32 active;  /* m_pBoss */
    /* 0x44 */ s32 percent; /* m_BossPercent */
} HudMeter; /* size 0x48 */

typedef struct {
    /* 0x00 */ HudMeter meters[2];
} HudMeters;

extern u8 D_800B2C50[];
extern void COverlay__StartScript(void *, void *);

void COnScreen__SetBoss(HudMeters *arg0, s32 arg1, s32 arg2) {
    arg0->meters[0].active = arg1;
    arg0->meters[0].unk3C = arg2;
    arg0->meters[0].percent = 100;
    COverlay__StartScript(arg0, D_800B2C50);
}
