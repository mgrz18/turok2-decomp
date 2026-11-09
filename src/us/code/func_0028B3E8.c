#include "common.h"
#include "m2c_macros.h"
#include "protos/ultra.h"


void Sched_SubmitTask(s32 arg0, s32 arg1) {
    osSendMesg(arg0 + 0x78, arg1, 1);
    osSendMesg(arg0 + 0x40, 6, 1);
}
