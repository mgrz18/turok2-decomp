#include "common.h"
#include "structs/boss.h"

extern f32 func_002119FC(f32 lo, f32 hi);

/* LevelScriptState from spawnsLeft on (D_8012F950, +0x348): this file
 * reaches script2 through that symbol. */
typedef struct {
    /* 0x00 */ s32 spawnsLeft;    /* 0x8012F950 */
    /* 0x04 */ s32 wavesLeft;
    /* 0x08 */ s32 enemiesAlive;
    /* 0x0C */ u8 pad0C[0xB8 - 0xC];
    /* 0xB8 */ BossScript2 script2; /* 0x8012FA08 */
    /* 0xC8 */ u8 script2On;      /* 0x8012FA18 */
} LevelScriptF950;

extern LevelScriptF950 D_8012F950;

/* Advance the second script to its next step, skipping steps by chance. */
void func_00404FE0(void) {
    if (D_8012F950.script2.steps == NULL) {
        return;
    }
    if (D_8012F950.script2.cursor != -1) {
        D_8012F950.script2.timer = D_8012F950.script2.steps[D_8012F950.script2.cursor].delay;
    } else {
        D_8012F950.script2.timer = 0.0f;
    }
    do {
        D_8012F950.script2.cursor++;
        if (D_8012F950.script2.steps[D_8012F950.script2.cursor].type == 4) {
            D_8012F950.script2On = 0;
            D_8012F950.script2.cursor = -1;
            D_8012F950.script2.current = -1;
            D_8012F950.script2.steps = NULL;
            return;
        }
        D_8012F950.script2.current = D_8012F950.script2.cursor;
    } while ((f32) D_8012F950.script2.steps[D_8012F950.script2.cursor].chance <= func_002119FC(0.0f, 100.0f));
}
