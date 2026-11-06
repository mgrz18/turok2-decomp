#include "common.h"
#include "m2c_macros.h"

extern s32 gSavedPlayers[];

void SavedPlayers_UpdateTimer(void) {
    s32 *p = gSavedPlayers;

    if (p[2] != 0) {
        p[2]--;
        if (p[2] == 0) {
            p[1] = 0;
        }
    }
}
