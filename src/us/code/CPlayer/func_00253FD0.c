#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"


s32 PlayerPool_GetNumPlayers(void) {
    return gEngineApp_m_Game.m_PlayerList.m_nPlayers;
}
