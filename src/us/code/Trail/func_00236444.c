#include "common.h"
#include "structs/swoosh.h"

void *NodePool_Alloc(void *pool);

/* Starts a timer: callback arg1 (with arg2) every arg4, arg3 times; the first at once. */
void SwooshSystem_AddTimer(CSwooshSystem *arg0, s32 (*arg1)(SwooshTimer *), sptr arg2, s32 arg3, f32 arg4) {
    SwooshTimer *temp_v0;

    temp_v0 = NodePool_Alloc(&arg0->m_TimerPool);
    if (temp_v0 != 0) {
        temp_v0->callback = arg1;
        temp_v0->arg = arg2;
        *(s32 *) &temp_v0->time = 0;
        temp_v0->period = arg4;
        temp_v0->repeats = arg3;
    }
}
