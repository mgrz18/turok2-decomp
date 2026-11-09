#include "common.h"
#include "m2c_macros.h"

u16 FloatToHalf(f32 arg0) {
#ifdef TARGET_PORT
    /* the float's top 16 bits (the N64 reads its first halfword: big-endian); HalfToFloat is the inverse */
    union {
        f32 f;
        u32 u;
    } v;
    v.f = arg0;
    return v.u >> 16;
#else
    return *(u16 *)&arg0;
#endif
}
