#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;
extern s32 D_8011ACA4;
extern f32 D_800B6D28;          /* frame time scale */

s32 func_00284174(void);
s32 func_00284240(void);
void func_002838A4(void);

/* Advances the clock at D_800C1BB0 by one frame. */
void func_00284568(void) {
    if (func_00284174()) {
        if (D_8011ACA4 != 0 || func_00284240()) {
            D_800C1BB0->prevTime = D_800C1BB0->time;
            return;
        }
        D_800C1BB0->prevTime = D_800C1BB0->time;
        D_800C1BB0->time += D_800B6D28 * (1.0f / 15);
        if (D_800C1BB0->time >= D_800C1BB0->startTime) {
            func_002838A4();
        }
        if (D_800C1BB0->time >= D_800C1BB0->clipEnd) {
            D_800C1BB0->time = D_800C1BB0->clipEnd;
            if (D_800C1BB0->stopAtEnd) {
                D_800C1BB0->finished = 1;
                D_800C1BB0->stopAtEnd = 0;
            }
        }
    }
}
