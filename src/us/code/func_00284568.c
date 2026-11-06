#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;
extern s32 D_8011ACA4;
extern f32 gFrameIncrement;          /* frame time scale */

s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);
void Cinema_Start(void);

/* Advances the clock at D_800C1BB0 by one frame. */
void Cinema_AdvanceClock(void) {
    if (Cinema_IsActive()) {
        if (D_8011ACA4 != 0 || Cinema_IsFinished()) {
            D_800C1BB0->prevTime = D_800C1BB0->time;
            return;
        }
        D_800C1BB0->prevTime = D_800C1BB0->time;
        D_800C1BB0->time += gFrameIncrement * (1.0f / 15);
        if (D_800C1BB0->time >= D_800C1BB0->startTime) {
            Cinema_Start();
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
