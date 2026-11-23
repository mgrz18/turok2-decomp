#include "common.h"
#include "globals/boss.h"

/* Two words at gLevelScript_script3_cursor; the pointer just before them (gLevelScript_script3_steps) picks
 * the data for the given mode. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} ModePair;


void Primagen_SetSoundScript(s32 arg0) {
    gLevelScript.script3.cursor = -1;
    gLevelScript.script3.current = -1;
    if (arg0 == 0) {
        gLevelScript.script3.steps = NULL;
    }
    if (arg0 == 1) {
        gLevelScript.script3.steps = sPrimagenSoundScript1;
    }
    if (arg0 == 4) {
        gLevelScript.script3.steps = sPrimagenSoundScript4;
    }
    if (arg0 == 2) {
        gLevelScript.script3.steps = sPrimagenSoundScript2;
    }
    if (arg0 == 3) {
        gLevelScript.script3.steps = sPrimagenSoundScript3;
    }
}
