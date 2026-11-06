#include "common.h"

extern s32 D_004312D4;
extern s32 D_00431344;
extern s32 D_004313EC;
extern s32 D_00431478;
extern s32 D_004314B0;
/* LevelScriptState.script2 (structs/boss.h, 0x8012FA08 on); the ROM addresses it
 * through D_8012FA10, one word in, so the script pointer is at index -1. */
typedef struct {
    /* 0x00 */ s32 scriptPrev;
    /* 0x04 */ s32 scriptIndex;
    /* 0x08 */ s8 restart;
} LevelScriptFA10;

extern LevelScriptFA10 D_8012FA10;

#define SCRIPT(state) (((s32 **) &(state))[-1])

void Primagen_SetSpawnScript_2(s32 arg0) {
    D_8012FA10.scriptPrev = -1;
    D_8012FA10.restart = 1;
    D_8012FA10.scriptIndex = -1;
    if (arg0 == 0) {
        SCRIPT(D_8012FA10) = NULL;
    }
    if (arg0 == 1) {
        SCRIPT(D_8012FA10) = &D_004312D4;
    }
    if (arg0 == 2) {
        SCRIPT(D_8012FA10) = &D_00431344;
    }
    if (arg0 == 3) {
        SCRIPT(D_8012FA10) = &D_004313EC;
    }
    if (arg0 == 4) {
        SCRIPT(D_8012FA10) = &D_00431478;
    }
    if (arg0 == 5) {
        SCRIPT(D_8012FA10) = &D_004314B0;
    }
}
