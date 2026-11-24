#include "common.h"

#include "structs/hud.h"

#include "structs/player.h"

/* Host body: the view stores m_pBoss's 0 at its N64 offset 0x40, which on the host (a
 * pointer-wide COverlay.m_pScript) is m_BossOverlay.m_Value: the boss was never cleared and its
 * bar stayed up after the fight (round 17 f4). The same store by member. */
void COnScreen__ClearBoss(void *arg0) {
    ((COnScreen *)arg0)->m_pBoss = NULL;
}
