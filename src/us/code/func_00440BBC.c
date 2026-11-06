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

extern LevelScriptF9D0 gLevelScript_scriptCOn; /* local type: the field's real type (or the member path) changes the ROM's code here */

void Primagen_EnterMode16_2(void) {
    gLevelScript_scriptCOn.active = 1;
    gLevelScript_scriptCOn.unk54 = -1;
    gLevelScript_scriptCOn.unk58 = -1;
    gLevelScript_scriptCOn.data = sPrimagenSoundScript4_2;
}
