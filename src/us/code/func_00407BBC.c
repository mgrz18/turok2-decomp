#include "common.h"

/* LevelScriptState (structs/boss.h) from scriptCOn on, gLevelScript_scriptCOn (+0x3C8):
 * scriptCOn and script3. */
typedef struct {
    /* 0x00 */ s8 restart;
    /* 0x01 */ u8 pad1[0x50 - 0x1];
    /* 0x50 */ s32 *script;
    /* 0x54 */ s32 scriptPrev;
    /* 0x58 */ s32 scriptIndex;
} LevelScriptF9D0;

extern s32 sPrimagenSoundScript4;
extern LevelScriptF9D0 gLevelScript_scriptCOn;

void Primagen_EnterMode16(void) {
    gLevelScript_scriptCOn.restart = 1;
    gLevelScript_scriptCOn.scriptPrev = -1;
    gLevelScript_scriptCOn.scriptIndex = -1;
    gLevelScript_scriptCOn.script = &sPrimagenSoundScript4;
}
