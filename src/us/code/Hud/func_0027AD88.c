#include "common.h"

#include "structs/hud.h"

#include "structs/player.h"

/* Host body: the view stores the percent at m_BossPercent's N64 offset 0x44, which on the host is
 * COverlay padding: the bar never went down (round 17 f4). The same store by member. */
void COnScreen__SetBossPercent(void *arg0, s32 arg1) {
    ((COnScreen *)arg0)->m_BossPercent = arg1;
}
