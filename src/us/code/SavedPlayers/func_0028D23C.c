#include "common.h"
#include "m2c_macros.h"

extern s32 gSavedPlayers[]; /* local type: globals/save.h declares it; this view (its type here) is what keeps the ROM's code */

void SavedPlayers_UpdateTimer(void) {
    s32 *p = gSavedPlayers;

    if (p[2] != 0) {
        p[2]--;
        if (p[2] == 0) {
            p[1] = 0;
        }
    }
}
