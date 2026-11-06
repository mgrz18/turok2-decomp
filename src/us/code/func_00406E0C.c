#include "common.h"
#include "structs/boss.h"

extern BossScript D_8012F9F8; /* LevelScriptState.script */
extern f32 Random_Blend(f32 lo, f32 hi);

/* Pick the next attack of the script: steps are skipped by chance, type 5 wraps around. */
void Primagen_NextAttackStep(void) {
    if (D_8012F9F8.cursor != -1) {
        D_8012F9F8.timer = D_8012F9F8.steps[D_8012F9F8.cursor].delay;
    } else {
        D_8012F9F8.timer = 0.0f;
    }
    do {
        D_8012F9F8.cursor++;
        if (D_8012F9F8.steps[D_8012F9F8.cursor].type == 5) {
            D_8012F9F8.cursor = 0;
        }
        D_8012F9F8.current = D_8012F9F8.cursor;
    } while ((f32) D_8012F9F8.steps[D_8012F9F8.cursor].chance <= Random_Blend(0.0f, 100.0f));
}
