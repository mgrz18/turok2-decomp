#include "common.h"
#include "globals/boss.h"

/* LevelScriptState.script2 (structs/boss.h, 0x8012FA08 on); the ROM addresses it
 * through gLevelScript_script2_cursor, one word in, so the script pointer is at index -1. */
typedef struct {
    /* 0x00 */ s32 scriptPrev;
    /* 0x04 */ s32 scriptIndex;
    /* 0x08 */ s8 restart;
} LevelScriptFA10;



void Primagen_SetSpawnScript_2(s32 arg0) {
    gLevelScript.script2.cursor = -1;
    gLevelScript.script2On = 1;
    gLevelScript.script2.current = -1;
    if (arg0 == 0) {
        gLevelScript.script2.steps = NULL;
    }
    if (arg0 == 1) {
        gLevelScript.script2.steps = sPrimagenSpawnScript1_2;
    }
    if (arg0 == 2) {
        gLevelScript.script2.steps = sPrimagenSpawnScript2_2;
    }
    if (arg0 == 3) {
        gLevelScript.script2.steps = sPrimagenSpawnScript3_2;
    }
    if (arg0 == 4) {
        gLevelScript.script2.steps = sPrimagenSpawnScript4_2;
    }
    if (arg0 == 5) {
        gLevelScript.script2.steps = sPrimagenSpawnScript5_2;
    }
}
