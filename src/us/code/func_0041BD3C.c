#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_8011AB18;

M2C_UNK *func_0041BD3C(void *arg0) {
    M2C_UNK *temp_v1_2;
    M2C_UNK *var_v0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x1C);
    var_v0 = &D_8011AB18;
    if (temp_v1 != NULL) {
        temp_v1_2 = M2C_FIELD(temp_v1, M2C_UNK **, 0x518);
        if (temp_v1_2 != NULL) {
            var_v0 = temp_v1_2;
        }
    }
    return var_v0;
}
