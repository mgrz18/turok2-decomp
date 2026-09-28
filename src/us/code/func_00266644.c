#include "common.h"
#include "m2c_macros.h"

s32 func_0021D928(s32, s32, s32);

extern f32 D_800A7CAC;

extern s32 D_800AB8B0;
extern f32 D_800AB8C0;

void func_00266644(s32 arg0, void *arg1, s32 arg2) {
    f32 var_f1;

    var_f1 = D_800A7CAC - (M2C_FIELD(arg1, f32 *, 0x40) * 8.5f);
    D_800AB8B0 = 1;
    if (var_f1 < 0.0f) {
        var_f1 = 0.0f;
    }
    D_800AB8C0 = var_f1;
    func_0021D928(arg0, (s32) arg1, arg2);
}
