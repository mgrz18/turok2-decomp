#include "common.h"
#include "m2c_macros.h"

extern s32 gSavedPlayers; /* local type: globals/save.h declares it; this view (its type here) is what keeps the ROM's code */

void SavedPlayers_Deactivate(void) {
    gSavedPlayers = 0;
}
