#include "common.h"
#include "globals/boss.h"

/* LevelScriptState (structs/boss.h) from scriptCOn on, gLevelScript_scriptCOn (+0x3C8):
 * scriptCOn and script3. */
#ifdef TARGET_PORT
typedef struct LevelScriptF9D0 {
/* the view at the host offsets of LevelScriptState (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_LevelScriptF9D0(m) __builtin_offsetof(LevelScriptState, m)
    union {
        struct { u8 _hp_restart[T2_HOFF_LevelScriptF9D0(scriptCOn) - T2_HOFF_LevelScriptF9D0(scriptCOn)]; s8 restart; }; /* N64 0x0 */
        struct { u8 _hp_script[T2_HOFF_LevelScriptF9D0(script3.steps) - T2_HOFF_LevelScriptF9D0(scriptCOn)]; BossStep3 *script; }; /* N64 0x50 */
        struct { u8 _hp_scriptPrev[T2_HOFF_LevelScriptF9D0(script3.cursor) - T2_HOFF_LevelScriptF9D0(scriptCOn)]; s32 scriptPrev; }; /* N64 0x54 */
        struct { u8 _hp_scriptIndex[T2_HOFF_LevelScriptF9D0(script3.current) - T2_HOFF_LevelScriptF9D0(scriptCOn)]; s32 scriptIndex; }; /* N64 0x58 */
    };
} LevelScriptF9D0;
#else
typedef struct {
    /* 0x00 */ s8 restart;
    /* 0x01 */ u8 pad1[0x50 - 0x1];
    /* 0x50 */ BossStep3 *script;
    /* 0x54 */ s32 scriptPrev;
    /* 0x58 */ s32 scriptIndex;
} LevelScriptF9D0;
#endif

extern LevelScriptF9D0 gLevelScript_scriptCOn; /* local type: the field's real type (or the member path) changes the ROM's code here */

void Primagen_EnterMode16(void) {
    gLevelScript_scriptCOn.restart = 1;
    gLevelScript_scriptCOn.scriptPrev = -1;
    gLevelScript_scriptCOn.scriptIndex = -1;
    gLevelScript_scriptCOn.script = sPrimagenSoundScript4;
}
