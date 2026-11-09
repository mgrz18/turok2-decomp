#include "common.h"
#include "m2c_macros.h"
#include "protos/ai.h"


void Action_EventIdle(s32 arg0, s32 arg1) {
    CAI__SetMode(arg0, arg1, 0);
}
