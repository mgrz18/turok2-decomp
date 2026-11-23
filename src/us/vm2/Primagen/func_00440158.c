#include "common.h"
#include "globals/boss.h"

/* gLevelScript_script3_cursor: two words reset to -1; the word just before it (gLevelScript_script3_steps) holds a table pointer. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
} State8012FA24;


void Primagen_SetSoundScript_2(s32 arg0) {
    gLevelScript.script3.cursor = -1;
    gLevelScript.script3.current = -1;
    if (arg0 == 0) {
        gLevelScript.script3.steps = NULL;
    }
    if (arg0 == 1) {
        gLevelScript.script3.steps = sPrimagenSoundScript1_2;
    }
    if (arg0 == 4) {
        gLevelScript.script3.steps = sPrimagenSoundScript4_2;
    }
    if (arg0 == 2) {
        gLevelScript.script3.steps = sPrimagenSoundScript2_2;
    }
    if (arg0 == 3) {
        gLevelScript.script3.steps = sPrimagenSoundScript3_2;
    }
}
