#include "common.h"
#include "m2c_macros.h"

s16 adecDecodeFrame();                                /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS adecDecodeFrame receives adecDecodeFrameOk's own argument
 * registers (pass-through); the host passes them. */
u32 adecDecodeFrameOk(void * pt0) {
    return (u32) ~adecDecodeFrame(pt0) >> 0x1F;
}
#else
u32 adecDecodeFrameOk(void) {
    return (u32) ~adecDecodeFrame() >> 0x1F;
}
#endif
