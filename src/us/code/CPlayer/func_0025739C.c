#include "common.h"
#include "structs/player.h"
#include "globals/game.h"
#include "structs/game.h"


void CPlayer__ToggleLowerAmmo(CPlayer *arg0) {
#ifdef TARGET_PORT
    /* the N64 reads the automap's first word (gEngineApp_m_Game_m_DebugLines, 0x51 bytes before the
     * multiplayer flag); on the host that distance lands elsewhere (round 18 g2, n64viewcheck gptr) */
    if ((gEngineApp_m_Settings.multiplayer == 0) && ((gEngineApp_m_Game_m_DebugLines.unk0 == 0) || !(arg0->m_ContState.m_dwButtons & 0x20)) && (arg0->m_ContState.m_dwButtonsSingle & 0x800)) {
#else
    if ((*(u8 *)&gEngineApp_m_Settings.multiplayer == 0) && ((*(s32 *)((u8 *)&gEngineApp_m_Settings.multiplayer - 0x51) == 0) || !(arg0->m_ContState.m_dwButtons & 0x20)) && (arg0->m_ContState.m_dwButtonsSingle & 0x800)) {
#endif
        if (arg0->m_UseLowerAmmo != 0) {
            arg0->m_UseLowerAmmo = 0;
            return;
        }
        arg0->m_UseLowerAmmo = 1;
    }
}
