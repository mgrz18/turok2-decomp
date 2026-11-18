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

#ifdef TARGET_PORT
#include "structs/player.h"

/* Host body: the view stores the percent at m_BossPercent's N64 offset 0x44, which on the host is
 * COverlay padding: the bar never went down (round 17 f4). The same store by member. */
void COnScreen__SetBossPercent(void *arg0, s32 arg1) {
    ((COnScreen *)arg0)->m_BossPercent = arg1;
}
#else
void COnScreen__SetBossPercent(ModeState *arg0, s32 arg1) {
    arg0->unk44 = arg1;
}
#endif
