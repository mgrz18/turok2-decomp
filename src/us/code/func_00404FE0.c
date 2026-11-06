#include "common.h"
#include "structs/boss.h"

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

extern LevelScriptF950 gLevelScript_spawnsLeft; /* local type: the field's real type (or the member path) changes the ROM's code here */

/* Advance the second script to its next step, skipping steps by chance. */
void Primagen_NextSpawnStep(void) {
    if (gLevelScript_spawnsLeft.script2.steps == NULL) {
        return;
    }
    if (gLevelScript_spawnsLeft.script2.cursor != -1) {
        gLevelScript_spawnsLeft.script2.timer = gLevelScript_spawnsLeft.script2.steps[gLevelScript_spawnsLeft.script2.cursor].delay;
    } else {
        gLevelScript_spawnsLeft.script2.timer = 0.0f;
    }
    do {
        gLevelScript_spawnsLeft.script2.cursor++;
        if (gLevelScript_spawnsLeft.script2.steps[gLevelScript_spawnsLeft.script2.cursor].type == 4) {
            gLevelScript_spawnsLeft.script2On = 0;
            gLevelScript_spawnsLeft.script2.cursor = -1;
            gLevelScript_spawnsLeft.script2.current = -1;
            gLevelScript_spawnsLeft.script2.steps = NULL;
            return;
        }
        gLevelScript_spawnsLeft.script2.current = gLevelScript_spawnsLeft.script2.cursor;
    } while ((f32) gLevelScript_spawnsLeft.script2.steps[gLevelScript_spawnsLeft.script2.cursor].chance <= Random_Blend(0.0f, 100.0f));
}
