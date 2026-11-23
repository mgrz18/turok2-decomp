#include "common.h"
#include "m2c_macros.h"

s16 adecDecodeFrame();                                /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS adecDecodeFrame receives adecDecodeFrameOk's own argument
 * registers (pass-through); the host passes them. */
u32 adecDecodeFrameOk(void * pt0) {
    return (u32) ~adecDecodeFrame(pt0) >> 0x1F;
}
