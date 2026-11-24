#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

void __scYield(OSSched *arg0) {
    OSScTask *temp_a0;

    temp_a0 = arg0->curRSPTask;
    if (temp_a0->list.t.type == 1) {
        temp_a0->state = (s32) (temp_a0->state | 0x10);
        osSpTaskYield();
    }
}
