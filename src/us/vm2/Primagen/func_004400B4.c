#include "common.h"
#include "globals/boss.h"

/* gLevelScript_script2_cursor: a small state block; the word before it (gLevelScript_script2_steps) is the
 * script pointer chosen here. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s8 unk8;
} State_004070B4;


#define SCRIPT gLevelScript.script2.steps

void Primagen_SetSpawnScript_2(s32 arg0) {
    gLevelScript.script2.cursor = -1;
    gLevelScript.script2On = 1;
    gLevelScript.script2.current = -1;
    if (arg0 == 0) {
        SCRIPT = NULL;
    }
    if (arg0 == 1) {
        SCRIPT = sPrimagenSpawnScript1_2;
    }
    if (arg0 == 2) {
        SCRIPT = sPrimagenSpawnScript2_2;
    }
    if (arg0 == 3) {
        SCRIPT = sPrimagenSpawnScript3_2;
    }
    if (arg0 == 4) {
        SCRIPT = sPrimagenSpawnScript4_2;
    }
    if (arg0 == 5) {
        SCRIPT = sPrimagenSpawnScript5_2;
    }
}
