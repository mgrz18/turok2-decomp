#include "common.h"
#include "structs/boss.h"

extern f32 Random_Blend(f32 lo, f32 hi);

/* LevelScriptState from spawnsLeft on (gLevelScript_spawnsLeft, +0x348): this file
 * reaches script2 through that symbol. */
#ifdef TARGET_PORT
typedef struct LevelScriptF950 {
/* the view at the host offsets of LevelScriptState (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_LevelScriptF950(m) __builtin_offsetof(LevelScriptState, m)
    union {
        struct { u8 _hp_spawnsLeft[T2_HOFF_LevelScriptF950(spawnsLeft) - T2_HOFF_LevelScriptF950(spawnsLeft)]; s32 spawnsLeft; }; /* N64 0x0 */
        struct { u8 _hp_wavesLeft[T2_HOFF_LevelScriptF950(wavesLeft) - T2_HOFF_LevelScriptF950(spawnsLeft)]; s32 wavesLeft; }; /* N64 0x4 */
        struct { u8 _hp_enemiesAlive[T2_HOFF_LevelScriptF950(enemiesAlive) - T2_HOFF_LevelScriptF950(spawnsLeft)]; s32 enemiesAlive; }; /* N64 0x8 */
        struct { u8 _hp_script2[T2_HOFF_LevelScriptF950(script2) - T2_HOFF_LevelScriptF950(spawnsLeft)]; BossScript2 script2; }; /* N64 0xB8 */
        struct { u8 _hp_script2On[T2_HOFF_LevelScriptF950(script2On) - T2_HOFF_LevelScriptF950(spawnsLeft)]; u8 script2On; }; /* N64 0xC8 */
    };
} LevelScriptF950;
#else
typedef struct {
    /* 0x00 */ s32 spawnsLeft;    /* 0x8012F950 */
    /* 0x04 */ s32 wavesLeft;
    /* 0x08 */ s32 enemiesAlive;
    /* 0x0C */ u8 pad0C[0xB8 - 0xC];
    /* 0xB8 */ BossScript2 script2; /* 0x8012FA08 */
    /* 0xC8 */ u8 script2On;      /* 0x8012FA18 */
} LevelScriptF950;
#endif

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
