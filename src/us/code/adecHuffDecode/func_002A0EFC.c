#include "common.h"
#include "structs/adec.h"

s32 adecDecodeFrame(AdecStream *st);

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS adecDecodeFrame received adecDecodeFrameOk's own argument
 * registers (pass-through); the host passes them. */
u32 adecDecodeFrameOk(AdecStream *st) {
#ifdef TARGET_PORT
    return (u32) ~adecDecodeFrame(st) >> 0x1F;
#else
    /* The ROM caller takes the result as an s16: called through a type of its own. */
    return (u32) ~((s16 (*)(AdecStream *))adecDecodeFrame)(st) >> 0x1F;
#endif
}
