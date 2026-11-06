#include "common.h"
#include "m2c_macros.h"

s32 CAI__SetMode(s32, s32, s32);

void Action_EventIdle(s32 arg0, s32 arg1) {
    CAI__SetMode(arg0, arg1, 0);
}
