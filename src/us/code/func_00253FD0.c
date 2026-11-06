#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Game_m_PlayerList_m_nPlayers;

s32 PlayerPool_GetNumPlayers(void) {
    return gEngineApp_m_Game_m_PlayerList_m_nPlayers;
}
