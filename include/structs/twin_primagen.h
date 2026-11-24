/* Local types that vm1/Primagen and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_PRIMAGEN_H
#define STRUCTS_TWIN_PRIMAGEN_H

#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "structs/boss.h"
#include "structs/vector.h"
#include "structs/light.h"
#include "structs/model.h"

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 pad01[0x8 - 0x1];
    /* 0x08 */ s32 count;
    /* 0x0C */ f32 delay;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ s32 type;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
} Wave0040457C;
typedef struct {
    /* 0x0 */ f32 waitMin;       /* 0x8012F618 */
    /* 0x4 */ f32 waitMax;       /* 0x8012F61C */
} BossWait;
typedef struct {
    u8 data[0x64];
} Slot00407DC0;

/* LevelScriptState from spawnsLeft on (gLevelScript_spawnsLeft, +0x348): Primagen_NextSpawnStep and
 * Primagen_RunSpawnStep reach script2 through that symbol. */
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

#endif
