#include "common.h"
#include "structs/player.h"

extern u8 gEngineApp_m_Settings_multiplayer;

void CPlayer__ResetLives(CPlayer *arg0) {
    if (gEngineApp_m_Settings_multiplayer != 0) {
        arg0->m_Data.m_Lives = 1;
        return;
    }
    arg0->m_Data.m_Lives = 3;
}
