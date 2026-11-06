#include "common.h"

extern s32 sPrimagenSpawnScript1_2;
extern s32 sPrimagenSpawnScript2_2;
extern s32 sPrimagenSpawnScript3_2;
extern s32 sPrimagenSpawnScript4_2;
extern s32 sPrimagenSpawnScript5_2;
/* LevelScriptState.script2 (structs/boss.h, 0x8012FA08 on); the ROM addresses it
 * through gLevelScript_script2_cursor, one word in, so the script pointer is at index -1. */
typedef struct {
    /* 0x00 */ s32 scriptPrev;
    /* 0x04 */ s32 scriptIndex;
    /* 0x08 */ s8 restart;
} LevelScriptFA10;

extern LevelScriptFA10 gLevelScript_script2_cursor;

#define SCRIPT(state) (((s32 **) &(state))[-1])

void Primagen_SetSpawnScript_2(s32 arg0) {
    gLevelScript_script2_cursor.scriptPrev = -1;
    gLevelScript_script2_cursor.restart = 1;
    gLevelScript_script2_cursor.scriptIndex = -1;
    if (arg0 == 0) {
        SCRIPT(gLevelScript_script2_cursor) = NULL;
    }
    if (arg0 == 1) {
        SCRIPT(gLevelScript_script2_cursor) = &sPrimagenSpawnScript1_2;
    }
    if (arg0 == 2) {
        SCRIPT(gLevelScript_script2_cursor) = &sPrimagenSpawnScript2_2;
    }
    if (arg0 == 3) {
        SCRIPT(gLevelScript_script2_cursor) = &sPrimagenSpawnScript3_2;
    }
    if (arg0 == 4) {
        SCRIPT(gLevelScript_script2_cursor) = &sPrimagenSpawnScript4_2;
    }
    if (arg0 == 5) {
        SCRIPT(gLevelScript_script2_cursor) = &sPrimagenSpawnScript5_2;
    }
}
