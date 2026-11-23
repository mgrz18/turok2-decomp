#include "common.h"
#include "m2c_macros.h"

#include "globals/save.h"

void SavedPlayers_UpdateTimer(void) {
    s32 *p = (s32 *) &gSavedPlayers;

    if (p[2] != 0) {
        p[2]--;
        if (p[2] == 0) {
            p[1] = 0;
        }
    }
}
