#include "common.h"
#include "m2c_macros.h"
#include "protos/ultra.h"


void Sched_SubmitTask(sptr arg0, sptr arg1) {
    osSendMesg(arg0 + 0x78, arg1, 1);
    osSendMesg(arg0 + 0x40, 6, 1);
}
