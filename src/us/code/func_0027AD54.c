#include "common.h"
#include "globals/hud.h"

s32 COverlay__StartScript(void *, s32 *);           /* extern */

typedef struct {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ u8 sub48[0x84 - 0x48];
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 unk8C;
} Unk0027AD54;

#include "structs/player.h"

/* Host body: the view above writes the second boss bar at N64 offsets (0x48 the overlay, 0x84 the
 * name, 0x88 the boss, 0x8C the percent); the host's COnScreen has a pointer-wide
 * COverlay.m_pScript, so they land in the wrong members (round 17 f4). The same stores by member. */
void COnScreen__SetBoss2(void *arg0, sptr arg1, sptr arg2) {
    COnScreen *os = (COnScreen *)arg0;

    os->m_pBoss2 = (struct CGameObjectInstance *)arg1;
    os->m_pBossName2 = (char *)arg2;
    os->m_BossPercent2 = 0x64;
    COverlay__StartScript(&os->m_BossOverlay2, sBossBar2OverlayScript);
}
