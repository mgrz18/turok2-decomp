/* Second VM module twin of Primagen_SetStage (same source, same link address). */
#include "common.h"
#include "structs/boss.h"

void Primagen_SetStage_2(s32 mode) {
    if (mode == 1) {
        gLevelScript.partsDone = 120;
        gLevelScript.values[0][3] = 0x1400;
        gLevelScript.values[0][4] = 0x1400;
        gLevelScript.values[0][6] = 0x1400;
        gLevelScript.values[0][5] = 0x1400;
        gLevelScript.values[1][3] = 0x1400;
        gLevelScript.values[1][4] = 0x1400;
        gLevelScript.values[1][6] = 0x1400;
        gLevelScript.values[1][5] = 0x1400;
        gLevelScript.values[0][2] = 0x3200;
        gLevelScript.values[0][0] = 0x5000;
        gLevelScript.values[1][2] = 0x3200;
        gLevelScript.values[1][0] = 0x5000;
        gLevelScript.values[0][1] = 0xC800;
        gLevelScript.values[1][1] = 0xC800;
        gLevelScript.spawnTimer = 30.0f;
    } else if (mode == 2) {
        gLevelScript.partsDone = 5;
        gLevelScript.values[0][2] = 0x3200;
        gLevelScript.values[0][0] = 0x5000;
        gLevelScript.values[1][2] = 0x3200;
        gLevelScript.values[1][0] = 0x5000;
    } else if (mode == 3) {
        gLevelScript.partsDone = 2;
        gLevelScript.values[0][1] = 0xC800;
        gLevelScript.values[1][1] = 0xC800;
    }
    gLevelScript.stage = mode;
    gLevelScript.scriptCOn = 0;
    gLevelScript.partActive = 0;
}
