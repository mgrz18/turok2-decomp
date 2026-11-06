/* Second VM module twin of Primagen_SetStage (same source, same link address). */
#include "common.h"
#include "structs/boss.h"

void Primagen_SetStage_2(s32 mode) {
    if (mode == 1) {
        D_8012F608.partsDone = 120;
        D_8012F608.values[0][3] = 0x1400;
        D_8012F608.values[0][4] = 0x1400;
        D_8012F608.values[0][6] = 0x1400;
        D_8012F608.values[0][5] = 0x1400;
        D_8012F608.values[1][3] = 0x1400;
        D_8012F608.values[1][4] = 0x1400;
        D_8012F608.values[1][6] = 0x1400;
        D_8012F608.values[1][5] = 0x1400;
        D_8012F608.values[0][2] = 0x3200;
        D_8012F608.values[0][0] = 0x5000;
        D_8012F608.values[1][2] = 0x3200;
        D_8012F608.values[1][0] = 0x5000;
        D_8012F608.values[0][1] = 0xC800;
        D_8012F608.values[1][1] = 0xC800;
        D_8012F608.spawnTimer = 30.0f;
    } else if (mode == 2) {
        D_8012F608.partsDone = 5;
        D_8012F608.values[0][2] = 0x3200;
        D_8012F608.values[0][0] = 0x5000;
        D_8012F608.values[1][2] = 0x3200;
        D_8012F608.values[1][0] = 0x5000;
    } else if (mode == 3) {
        D_8012F608.partsDone = 2;
        D_8012F608.values[0][1] = 0xC800;
        D_8012F608.values[1][1] = 0xC800;
    }
    D_8012F608.stage = mode;
    D_8012F608.scriptCOn = 0;
    D_8012F608.partActive = 0;
}
