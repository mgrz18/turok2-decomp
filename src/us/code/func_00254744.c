#include "common.h"
#include "structs/player.h"
#include "globals/game.h"


void CPlayer__ResetLives(CPlayer *arg0) {
    if (gEngineApp_m_Settings.multiplayer != 0) {
        arg0->m_Data.m_Lives = 1;
        return;
    }
    arg0->m_Data.m_Lives = 3;
}
