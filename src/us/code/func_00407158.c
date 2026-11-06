#include "common.h"
#include "globals/boss.h"

/* Two words at gLevelScript_script3_cursor; the pointer just before them (gLevelScript_script3_steps) picks
 * the data for the given mode. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} ModePair;

extern ModePair gLevelScript_script3_cursor; /* local type: the field's real type (or the member path) changes the ROM's code here */

void Primagen_SetSoundScript(s32 arg0) {
    gLevelScript_script3_cursor.unk0 = -1;
    gLevelScript_script3_cursor.unk4 = -1;
    if (arg0 == 0) {
        ((BossStep3 **) &gLevelScript_script3_cursor)[-1] = NULL;
    }
    if (arg0 == 1) {
        ((BossStep3 **) &gLevelScript_script3_cursor)[-1] = sPrimagenSoundScript1;
    }
    if (arg0 == 4) {
        ((BossStep3 **) &gLevelScript_script3_cursor)[-1] = sPrimagenSoundScript4;
    }
    if (arg0 == 2) {
        ((BossStep3 **) &gLevelScript_script3_cursor)[-1] = sPrimagenSoundScript2;
    }
    if (arg0 == 3) {
        ((BossStep3 **) &gLevelScript_script3_cursor)[-1] = sPrimagenSoundScript3;
    }
}
