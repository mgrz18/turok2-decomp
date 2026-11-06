#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern f32 Random_Blend(f32 lo, f32 hi);

/* Pick the next attack of the script: steps are skipped by chance, type 5 wraps around. */
void Primagen_NextAttackStep(void) {
    if (gLevelScript.script.cursor != -1) {
        gLevelScript.script.timer = gLevelScript.script.steps[gLevelScript.script.cursor].delay;
    } else {
        gLevelScript.script.timer = 0.0f;
    }
    do {
        gLevelScript.script.cursor++;
        if (gLevelScript.script.steps[gLevelScript.script.cursor].type == 5) {
            gLevelScript.script.cursor = 0;
        }
        gLevelScript.script.current = gLevelScript.script.cursor;
    } while ((f32) gLevelScript.script.steps[gLevelScript.script.cursor].chance <= Random_Blend(0.0f, 100.0f));
}
