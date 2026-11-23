#include "common.h"
#include "globals/boss.h"

/* LevelScriptState (structs/boss.h) from scriptCOn on, gLevelScript_scriptCOn (+0x3C8). */
typedef struct {
    /* 0x00 */ s8 active;
    /* 0x01 */ u8 pad1[0x50 - 0x1];
    /* 0x50 */ BossStep3 *data;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
} LevelScriptF9D0;


void Primagen_EnterMode16_2(void) {
    gLevelScript.scriptCOn = 1;
    gLevelScript.script3.cursor = -1;
    gLevelScript.script3.current = -1;
    gLevelScript.script3.steps = sPrimagenSoundScript4_2;
}
