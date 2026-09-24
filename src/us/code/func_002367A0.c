#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0026D474(void *, void *);              /* extern */
extern f32 D_800B6D28;

void func_002367A0(void *arg0) {
    f32 temp_f0;
    f32 temp_f1;
    s32 temp_v0;
    void *temp_s1;
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x758C);
    if (var_s0 != NULL) {
        do {
            temp_f1 = M2C_FIELD(var_s0, f32 *, 8) - D_800B6D28;
            temp_s1 = M2C_FIELD(var_s0, void **, 4);
            M2C_FIELD(var_s0, f32 *, 8) = temp_f1;
            if (temp_f1 <= 0.0f) {
                temp_f0 = temp_f1 + M2C_FIELD(var_s0, f32 *, 0xC);
                M2C_FIELD(var_s0, f32 *, 8) = temp_f0;
                if (temp_f0 < 0.0f) {
                    M2C_FIELD(var_s0, f32 *, 8) = 0.0f;
                }
                M2C_FIELD(var_s0, M2C_UNK (**)(void *), 0x18)(var_s0);
                temp_v0 = M2C_FIELD(var_s0, s32 *, 0x10) - 1;
                M2C_FIELD(var_s0, s32 *, 0x10) = temp_v0;
                if (temp_v0 <= 0) {
                    func_0026D474(arg0 + 0x7580, var_s0);
                }
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}
