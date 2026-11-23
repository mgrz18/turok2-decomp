#include "common.h"
#include "m2c_macros.h"

#include "globals/save.h"

void SavedPlayers_Deactivate(void) {
    gSavedPlayers.m_bActive = 0;
}
