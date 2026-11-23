#include "common.h"
#include "structs/player.h"
#include "globals/save.h"

void SavedPlayers_Clear(void) {
    gSavedPlayers.m_bActive = 0;
    gSavedPlayers.m_State = 0;
    gSavedPlayers.m_Timer = 0;
}
