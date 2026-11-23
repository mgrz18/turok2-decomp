#include "common.h"

/* The state COverlay__StartScript sets up: a value and a mode, and two more words. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 value;
    /* 0x08 */ s32 mode;
    /* 0x0C */ u8 padC[0x40 - 0xC];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
} ModeState;

#include "structs/player.h"

/* Host body: the view stores m_pBoss's 0 at its N64 offset 0x40, which on the host (a
 * pointer-wide COverlay.m_pScript) is m_BossOverlay.m_Value: the boss was never cleared and its
 * bar stayed up after the fight (round 17 f4). The same store by member. */
void COnScreen__ClearBoss(void *arg0) {
    ((COnScreen *)arg0)->m_pBoss = NULL;
}
