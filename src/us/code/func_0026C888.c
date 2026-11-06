#include "common.h"
#include "m2c_macros.h"

extern f32 CGameRegion__FloodSetHeight(void *, f32, u16);

f32 CGameRegion__SetGroupHeight(u16 *arg0, f32 arg1) {
    f32 var_f0;

    var_f0 = arg1;
    if (arg0 != NULL) {
        var_f0 = CGameRegion__FloodSetHeight(arg0, arg1, *arg0);
    }
    return var_f0;
}
