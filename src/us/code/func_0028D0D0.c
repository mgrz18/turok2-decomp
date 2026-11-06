#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

s32 SavedPlayers_GetWarpID(void) {
    return gSavedPlayers.m_WarpID;
}
