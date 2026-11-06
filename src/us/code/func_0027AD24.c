#include "common.h"
#include "structs/player.h"
#include "globals/hud.h"

extern void COverlay__StartScript(COverlay *, s32 *);

void COnScreen__SetBoss(COnScreen *arg0, CGameObjectInstance *arg1, char *arg2) {
    arg0->m_pBoss = arg1;
    arg0->m_pBossName = arg2;
    arg0->m_BossPercent = 100;
    COverlay__StartScript(&arg0->m_BossOverlay, sBossBarOverlayScript);
}
