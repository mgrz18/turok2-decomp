#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern f32 Random_Blend(f32 lo, f32 hi);


/* Advance the third script to its next step, skipping steps by chance. */
void Primagen_NextSoundStep(void) {
    if (gLevelScript.script3.steps == NULL) {
        return;
    }
    if (gLevelScript.script3.cursor != -1) {
        gLevelScript.script3.timer = gLevelScript.script3.steps[gLevelScript.script3.cursor].delay
                         + Random_Blend(0.0f, gLevelScript.script3.steps[gLevelScript.script3.cursor].delayRange) * 15.0f;
    } else {
        gLevelScript.script3.timer = 0.0f;
    }
    do {
        gLevelScript.script3.cursor++;
        if (gLevelScript.script3.steps[gLevelScript.script3.cursor].type == 4) {
            gLevelScript.script3.cursor = -1;
            gLevelScript.script3.current = -1;
            gLevelScript.script3.steps = NULL;
            return;
        }
        gLevelScript.script3.current = gLevelScript.script3.cursor;
    } while ((f32) gLevelScript.script3.steps[gLevelScript.script3.cursor].chance <= Random_Blend(0.0f, 100.0f));
}
