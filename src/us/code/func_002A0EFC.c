#include "common.h"
#include "m2c_macros.h"

s16 adecDecodeFrame();                                /* extern */

u32 adecDecodeFrameOk(void) {
    return (u32) ~adecDecodeFrame() >> 0x1F;
}
