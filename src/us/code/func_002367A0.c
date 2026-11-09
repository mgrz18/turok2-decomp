#include "common.h"
#include "globals/engine.h"
#include "structs/swoosh.h"

s32 NodePool_Free(void *, void *);              /* extern */

/* Runs the repeating timers: each counts down by the frame time, fires its callback,
 * and is freed after its last repeat. */
void SwooshSystem_UpdateTimers(CSwooshSystem *arg0) {
    f32 temp_f0;
    f32 temp_f1;
    s32 temp_v0;
    SwooshTimer *temp_s1;
    SwooshTimer *var_s0;

    var_s0 = arg0->m_TimerPool.used.head;
    if (var_s0 != NULL) {
        do {
            temp_f1 = var_s0->time - gFrameIncrement;
            temp_s1 = var_s0->next;
            var_s0->time = temp_f1;
            if (temp_f1 <= 0.0f) {
                temp_f0 = temp_f1 + var_s0->period;
                var_s0->time = temp_f0;
                if (temp_f0 < 0.0f) {
                    var_s0->time = 0.0f;
                }
                var_s0->callback(var_s0);
                temp_v0 = var_s0->repeats - 1;
                var_s0->repeats = temp_v0;
                if (temp_v0 <= 0) {
                    NodePool_Free(&arg0->m_TimerPool, var_s0);
                }
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}
