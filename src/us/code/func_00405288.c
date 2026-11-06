#include "common.h"
#include "structs/boss.h"

extern f32 Random_Blend(f32 lo, f32 hi);


/* Advance the third script to its next step, skipping steps by chance. */
void Primagen_NextSoundStep(void) {
    if (D_8012FA1C.steps == NULL) {
        return;
    }
    if (D_8012FA1C.cursor != -1) {
        D_8012FA1C.timer = D_8012FA1C.steps[D_8012FA1C.cursor].delay
                         + Random_Blend(0.0f, D_8012FA1C.steps[D_8012FA1C.cursor].delayRange) * 15.0f;
    } else {
        D_8012FA1C.timer = 0.0f;
    }
    do {
        D_8012FA1C.cursor++;
        if (D_8012FA1C.steps[D_8012FA1C.cursor].type == 4) {
            D_8012FA1C.cursor = -1;
            D_8012FA1C.current = -1;
            D_8012FA1C.steps = NULL;
            return;
        }
        D_8012FA1C.current = D_8012FA1C.cursor;
    } while ((f32) D_8012FA1C.steps[D_8012FA1C.cursor].chance <= Random_Blend(0.0f, 100.0f));
}
