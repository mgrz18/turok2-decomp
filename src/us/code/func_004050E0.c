#include "common.h"
#include "structs/boss.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern f32 Random_Blend(f32 lo, f32 hi);

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

s32 Primagen_RunSpawnStep(void) {
    if (D_8012F950.script2.timer > 0.0f) {
        return 0;
    }
    if (!D_8012F950.script2On) {
        return 0;
    }
    if (D_8012F950.script2.steps == NULL) {
        return 0;
    }
    switch (D_8012F950.script2.steps[D_8012F950.script2.current].type) {
        case 1:
            if (D_8012F950.enemiesAlive < 9) {
                D_8012F950.spawnsLeft = MIN(D_8012F950.script2.steps[D_8012F950.script2.current].arg, 8 - D_8012F950.enemiesAlive);
            }
            D_8012F950.script2On = 0;
            return 1;
        case 2:
            D_8012F950.wavesLeft = D_8012F950.script2.steps[D_8012F950.script2.current].arg;
            D_8012F950.script2On = 0;
            return 1;
        case 5:
            D_8012F950.script2.current = -1;
            return 1;
        case 3:
            if (Random_Blend(0.0f, 100.0f) < 50.0f) {
                if (D_8012F950.enemiesAlive < 9) {
                    D_8012F950.spawnsLeft = MIN(D_8012F950.script2.steps[D_8012F950.script2.current].arg, 8 - D_8012F950.enemiesAlive);
                }
            } else {
                D_8012F950.wavesLeft = D_8012F950.script2.steps[D_8012F950.script2.current].arg;
            }
            D_8012F950.script2On = 0;
            return 1;
        case 4:
            break;
    }
    return 0;
}
