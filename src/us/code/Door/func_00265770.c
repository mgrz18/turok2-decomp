#include "common.h"
#include "m2c_macros.h"
#include "protos/ai.h"


void Door_EventOpen(sptr arg0, sptr arg1) {
    CAI__SetMode(arg0, arg1, 0);
}
