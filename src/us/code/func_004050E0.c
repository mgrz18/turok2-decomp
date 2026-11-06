#include "common.h"
#include "structs/boss.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern f32 Random_Blend(f32 lo, f32 hi);

/* LevelScriptState from spawnsLeft on (gLevelScript_spawnsLeft, +0x348): this file
 * reaches script2 through that symbol. */
typedef struct {
    /* 0x00 */ s32 spawnsLeft;    /* 0x8012F950 */
    /* 0x04 */ s32 wavesLeft;
    /* 0x08 */ s32 enemiesAlive;
    /* 0x0C */ u8 pad0C[0xB8 - 0xC];
    /* 0xB8 */ BossScript2 script2; /* 0x8012FA08 */
    /* 0xC8 */ u8 script2On;      /* 0x8012FA18 */
} LevelScriptF950;

extern LevelScriptF950 gLevelScript_spawnsLeft;

s32 Primagen_RunSpawnStep(void) {
    if (gLevelScript_spawnsLeft.script2.timer > 0.0f) {
        return 0;
    }
    if (!gLevelScript_spawnsLeft.script2On) {
        return 0;
    }
    if (gLevelScript_spawnsLeft.script2.steps == NULL) {
        return 0;
    }
    switch (gLevelScript_spawnsLeft.script2.steps[gLevelScript_spawnsLeft.script2.current].type) {
        case 1:
            if (gLevelScript_spawnsLeft.enemiesAlive < 9) {
                gLevelScript_spawnsLeft.spawnsLeft = MIN(gLevelScript_spawnsLeft.script2.steps[gLevelScript_spawnsLeft.script2.current].arg, 8 - gLevelScript_spawnsLeft.enemiesAlive);
            }
            gLevelScript_spawnsLeft.script2On = 0;
            return 1;
        case 2:
            gLevelScript_spawnsLeft.wavesLeft = gLevelScript_spawnsLeft.script2.steps[gLevelScript_spawnsLeft.script2.current].arg;
            gLevelScript_spawnsLeft.script2On = 0;
            return 1;
        case 5:
            gLevelScript_spawnsLeft.script2.current = -1;
            return 1;
        case 3:
            if (Random_Blend(0.0f, 100.0f) < 50.0f) {
                if (gLevelScript_spawnsLeft.enemiesAlive < 9) {
                    gLevelScript_spawnsLeft.spawnsLeft = MIN(gLevelScript_spawnsLeft.script2.steps[gLevelScript_spawnsLeft.script2.current].arg, 8 - gLevelScript_spawnsLeft.enemiesAlive);
                }
            } else {
                gLevelScript_spawnsLeft.wavesLeft = gLevelScript_spawnsLeft.script2.steps[gLevelScript_spawnsLeft.script2.current].arg;
            }
            gLevelScript_spawnsLeft.script2On = 0;
            return 1;
        case 4:
            break;
    }
    return 0;
}
