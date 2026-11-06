#include "common.h"
#include "structs/boss.h"

extern BossScript gLevelScript_script; /* LevelScriptState.script */
extern f32 Random_Blend(f32 lo, f32 hi);

/* Pick the next attack of the script: steps are skipped by chance, type 5 wraps around. */
void Primagen_NextAttackStep(void) {
    if (gLevelScript_script.cursor != -1) {
        gLevelScript_script.timer = gLevelScript_script.steps[gLevelScript_script.cursor].delay;
    } else {
        gLevelScript_script.timer = 0.0f;
    }
    do {
        gLevelScript_script.cursor++;
        if (gLevelScript_script.steps[gLevelScript_script.cursor].type == 5) {
            gLevelScript_script.cursor = 0;
        }
        gLevelScript_script.current = gLevelScript_script.cursor;
    } while ((f32) gLevelScript_script.steps[gLevelScript_script.cursor].chance <= Random_Blend(0.0f, 100.0f));
}
