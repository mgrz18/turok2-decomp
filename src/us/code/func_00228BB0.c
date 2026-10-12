#include "common.h"
#include "m2c_macros.h"

void CSunFrameData__Construct(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
}
