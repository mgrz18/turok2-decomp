#include "common.h"
#include "m2c_macros.h"

extern f32 D_800A77EC;
extern f32 D_800A77F0;

f32 func_0025EAFC(void *arg0) {
    f32 var_f0;

    var_f0 = D_800A77EC;
    if (M2C_FIELD(arg0, u16 *, 0xB8) != 0x259) {
        var_f0 = D_800A77F0;
    }
    return var_f0;
}
