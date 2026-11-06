#include "common.h"
#include "structs/player.h"
#include "globals/game.h"


void CPlayer__ToggleLowerAmmo(CPlayer *arg0) {
    if ((*(u8 *)&gEngineApp_m_Settings.multiplayer == 0) && ((*(s32 *)((u8 *)&gEngineApp_m_Settings.multiplayer - 0x51) == 0) || !(arg0->m_ContState.m_dwButtons & 0x20)) && (arg0->m_ContState.m_dwButtonsSingle & 0x800)) {
        if (arg0->m_UseLowerAmmo != 0) {
            arg0->m_UseLowerAmmo = 0;
            return;
        }
        arg0->m_UseLowerAmmo = 1;
    }
}
