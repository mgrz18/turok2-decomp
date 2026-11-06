#include "common.h"

void __assert(void *, void *, s32); /* extern */

s32 _DivS32ByF32(s32 arg0, f32 arg1) {
    f64 temp_f0;
    s32 var_v0;

    if (arg1 == 0.0f) {
        __assert("EX", "audio/sndplayer.c", 0x11D);
    }
    temp_f0 = (f64) ((f32) arg0 / arg1);
    var_v0 = 0x7FFFFFFF;
    if (!(2147483647.0 < temp_f0)) {
        var_v0 = (s32) temp_f0;
    }
    return var_v0;
}
