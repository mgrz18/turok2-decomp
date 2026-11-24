#include "common.h"
#include "structs/boss.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern f32 Random_Blend(f32 lo, f32 hi);

/* LevelScriptState from spawnsLeft on (gLevelScript_spawnsLeft, +0x348): this file
 * reaches script2 through that symbol. */
#include "structs/twin_primagen.h"

extern LevelScriptF950 gLevelScript_spawnsLeft_v004050E0 __asm__("gLevelScript_spawnsLeft"); /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 Primagen_RunSpawnStep(void) {
    if (gLevelScript_spawnsLeft_v004050E0.script2.timer > 0.0f) {
        return 0;
    }
    if (!gLevelScript_spawnsLeft_v004050E0.script2On) {
        return 0;
    }
    if (gLevelScript_spawnsLeft_v004050E0.script2.steps == NULL) {
        return 0;
    }
    switch (gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].type) {
        case 1:
            if (gLevelScript_spawnsLeft_v004050E0.enemiesAlive < 9) {
                gLevelScript_spawnsLeft_v004050E0.spawnsLeft = MIN(gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].arg, 8 - gLevelScript_spawnsLeft_v004050E0.enemiesAlive);
            }
            gLevelScript_spawnsLeft_v004050E0.script2On = 0;
            return 1;
        case 2:
            gLevelScript_spawnsLeft_v004050E0.wavesLeft = gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].arg;
            gLevelScript_spawnsLeft_v004050E0.script2On = 0;
            return 1;
        case 5:
            gLevelScript_spawnsLeft_v004050E0.script2.current = -1;
            return 1;
        case 3:
            if (Random_Blend(0.0f, 100.0f) < 50.0f) {
                if (gLevelScript_spawnsLeft_v004050E0.enemiesAlive < 9) {
                    gLevelScript_spawnsLeft_v004050E0.spawnsLeft = MIN(gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].arg, 8 - gLevelScript_spawnsLeft_v004050E0.enemiesAlive);
                }
            } else {
                gLevelScript_spawnsLeft_v004050E0.wavesLeft = gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].arg;
            }
            gLevelScript_spawnsLeft_v004050E0.script2On = 0;
            return 1;
        case 4:
            break;
    }
    return 0;
}
