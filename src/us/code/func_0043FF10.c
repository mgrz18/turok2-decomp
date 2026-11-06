#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

void Primagen_SetAttackScript_2(s32 arg0) {
    if (arg0 == 2) {
        if (gLevelScript.stage == 1) {
            gLevelScript.script.steps = sPrimagenAttackScript2Stage1_2;
        }
        if (gLevelScript.stage == arg0) {
            gLevelScript.script.steps = sPrimagenAttackScript2Stage2_2;
        }
        if (gLevelScript.stage == 3) {
            gLevelScript.script.steps = sPrimagenAttackScript2Stage3_2;
        }
        gLevelScript.script.cursor = -1;
        gLevelScript.script.current = -1;
    }
    if (arg0 == 1) {
        if (gLevelScript.stage == arg0) {
            gLevelScript.script.steps = sPrimagenAttackScriptStage1_2;
        }
        if (gLevelScript.stage == 2) {
            gLevelScript.script.steps = sPrimagenAttackScriptStage2_2;
        }
        if (gLevelScript.stage == 3) {
            gLevelScript.script.steps = sPrimagenAttackScriptStage3_2;
        }
        gLevelScript.script.cursor = -1;
        gLevelScript.script.current = -1;
    }
}
