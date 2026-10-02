#include "common.h"
#include "m2c_macros.h"

void func_0044FAA4(void *arg0) {
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 4);
    if (var_s0 != NULL) {
        do {
            M2C_FIELD(M2C_FIELD(var_s0, void **, 0x14), M2C_UNK (**)(void *, void *), 0x10)(var_s0, arg0);
            var_s0 = M2C_FIELD(var_s0, void **, 0x1D0);
        } while (var_s0 != NULL);
    }
}
