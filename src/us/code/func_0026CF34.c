#include "common.h"
#include "m2c_macros.h"

void func_0026CF34(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f0_3;
    f32 var_f0_4;
    f32 var_f1;
    f32 var_f1_2;
    f32 var_f1_3;
    f32 var_f1_4;

    var_f1 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 0);
    temp_f0 = M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 0);
    if (!(var_f1 <= temp_f0)) {
        var_f1 = temp_f0;
    }
    var_f0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0xC), f32 *, 0);
    if (!(var_f0 <= var_f1)) {
        var_f0 = var_f1;
    }
    M2C_FIELD(arg1, f32 *, 0) = var_f0;
    var_f1_2 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 0);
    temp_f0_2 = M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 0);
    if (!(temp_f0_2 <= var_f1_2)) {
        var_f1_2 = temp_f0_2;
    }
    var_f0_2 = M2C_FIELD(M2C_FIELD(arg0, void **, 0xC), f32 *, 0);
    if (!(var_f1_2 <= var_f0_2)) {
        var_f0_2 = var_f1_2;
    }
    M2C_FIELD(arg1, f32 *, 8) = var_f0_2;
    var_f1_3 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 8);
    temp_f0_3 = M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 8);
    if (!(var_f1_3 <= temp_f0_3)) {
        var_f1_3 = temp_f0_3;
    }
    var_f0_3 = M2C_FIELD(M2C_FIELD(arg0, void **, 0xC), f32 *, 8);
    if (!(var_f0_3 <= var_f1_3)) {
        var_f0_3 = var_f1_3;
    }
    M2C_FIELD(arg1, f32 *, 4) = var_f0_3;
    var_f1_4 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 8);
    temp_f0_4 = M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 8);
    if (!(temp_f0_4 <= var_f1_4)) {
        var_f1_4 = temp_f0_4;
    }
    var_f0_4 = M2C_FIELD(M2C_FIELD(arg0, void **, 0xC), f32 *, 8);
    if (!(var_f1_4 <= var_f0_4)) {
        var_f0_4 = var_f1_4;
    }
    M2C_FIELD(arg1, f32 *, 0xC) = var_f0_4;
}
