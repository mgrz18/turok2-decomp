#include "common.h"
#include "structs/boss.h"

void CAI__SetMode(s32 a0, s32 a1, s32 a2);
void Primagen_SetStage(s32 mode);

void Primagen_UpdateHealth(s32 a0, s32 a1) {
    s32 count;
    s32 i;
    s32 bit;

    count = 0;
    gLevelScript.bonus = 0;
    gLevelScript.total = 0;
    for (i = 0; i < 8; i++) {
        bit = 1 << i;
        if (gLevelScript.partsDone & bit) {
            gLevelScript.bonus += gLevelScript.values[1][i];
            if (gLevelScript.values[0][i] > 0) {
                gLevelScript.total += gLevelScript.values[0][i];
                count++;
            }
        }
    }
    if (gLevelScript.partActive == 0) {
        if (gLevelScript.stage == 3) {
            if (count == 0) {
                CAI__SetMode(a0, a1, 0x1D);
            }
        } else if (count == 0) {
            gLevelScript.stage++;
            if (gLevelScript.stage >= 2) {
                gLevelScript.bossMode = 3;
                gLevelScript.bossState = 3;
                gLevelScript.armed = 0;
                gLevelScript.scriptOn = 0;
                gLevelScript.dest.x = gLevelScript.home.x;
                gLevelScript.dest.y = gLevelScript.home.y;
                gLevelScript.dest.z = gLevelScript.home.z;
                CAI__SetMode(a0, a1, 0x12);
            }
            Primagen_SetStage(gLevelScript.stage);
        }
    }
}
