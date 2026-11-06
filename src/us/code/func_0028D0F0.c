#include "common.h"
#include "m2c_macros.h"

extern s32 gSavedPlayers;

void SavedPlayers_Deactivate(void) {
    gSavedPlayers = 0;
}
