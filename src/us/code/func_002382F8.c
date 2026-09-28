#include "common.h"
#include "m2c_macros.h"

extern f32 func_0021F268(s32);
extern f32 D_800A65B4[];

f32 func_002382F8(u8 *arg0) {
    if (*arg0 == 1) {
        return func_0021F268((s32) arg0);
    }
    return D_800A65B4[0];
}
