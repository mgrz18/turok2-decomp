#include "common.h"
#include "m2c_macros.h"
#include "protos/ai.h"


void Action_EventGo(s32 arg0, s32 arg1) {
    CAI__SetMode(arg0, arg1, 1);
}
