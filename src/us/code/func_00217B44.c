#include "common.h"
#ifdef TARGET_PORT
/* bit addresses are offsets from the stream base on the host (func_00217B04.c) */
extern unsigned long gT2BitStreamBase;
#endif
#include "m2c_macros.h"

s32 BitAddr_ToWordPointer(s32 arg0) {
#ifdef TARGET_PORT
    return (gT2BitStreamBase + (((u32) arg0 >> 3) & ~3u));
#else
    return (arg0 & 0xF0000000) | ((u32) (arg0 & 0x0FFFFFE0) >> 3);
#endif
}
