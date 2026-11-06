#include "common.h"

/* gLevelScript_script2_cursor: a small state block; the word before it (gLevelScript_script2_steps) is the
 * script pointer chosen here. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s8 unk8;
} State;

extern s32 sPrimagenSpawnScript1;
extern s32 sPrimagenSpawnScript2;
extern s32 sPrimagenSpawnScript3;
extern s32 sPrimagenSpawnScript4;
extern s32 sPrimagenSpawnScript5;
extern State gLevelScript_script2_cursor;

#define SCRIPT (((s32 **) &gLevelScript_script2_cursor)[-1])

void Primagen_SetSpawnScript(s32 arg0) {
    gLevelScript_script2_cursor.unk0 = -1;
    gLevelScript_script2_cursor.unk8 = 1;
    gLevelScript_script2_cursor.unk4 = -1;
    if (arg0 == 0) {
        SCRIPT = NULL;
    }
    if (arg0 == 1) {
        SCRIPT = &sPrimagenSpawnScript1;
    }
    if (arg0 == 2) {
        SCRIPT = &sPrimagenSpawnScript2;
    }
    if (arg0 == 3) {
        SCRIPT = &sPrimagenSpawnScript3;
    }
    if (arg0 == 4) {
        SCRIPT = &sPrimagenSpawnScript4;
    }
    if (arg0 == 5) {
        SCRIPT = &sPrimagenSpawnScript5;
    }
}
