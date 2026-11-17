#include "common.h"
#include "globals/game.h"

void Hud_SetupTextureMode(u8);
void COnScreen__DrawBossBar(void *, s32);

#ifdef TARGET_PORT
#include "structs/player.h"

/* Host body: the ROM tests m_pBoss and m_pBoss2 at their N64 offsets (0x40, 0x88). On the host
 * COverlay holds a pointer-wide m_pScript, so 0x40 is m_BossOverlay.m_Value (0 until the bar is
 * drawn: never) and 0x88 lands inside m_BossOverlay2: no boss bar was ever drawn (round 17 f4,
 * the Primagen's green bar, seen against the N64 from the same warp). The same tests by member. */
void COnScreen__DrawBossBars(void *arg0) {
    COnScreen *os = (COnScreen *)arg0;

    Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
    if (os->m_pBoss != NULL) {
        COnScreen__DrawBossBar(arg0, 1);
    }
    if (os->m_pBoss2 != NULL) {
        COnScreen__DrawBossBar(arg0, 2);
    }
}
#else
void COnScreen__DrawBossBars(void *arg0) {
    u8 pad[0x100];

    Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
    if (*(s32 *)((u8 *)arg0 + 0x40) != 0) {
        COnScreen__DrawBossBar(arg0, 1);
    }
    if (*(s32 *)((u8 *)arg0 + 0x88) != 0) {
        COnScreen__DrawBossBar(arg0, 2);
    }
}
#endif
