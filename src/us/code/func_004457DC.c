#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);
s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(M2C_UNK *);                     /* extern */
extern M2C_UNK D_80119870;
extern s32 D_8012FB70;

void func_004457DC(void) {
    void *temp_v0;
    void *temp_v1;

    func_00243414(D_8012FB70, D_8012FB70 + 0x140, 1);
    temp_v0 = func_002532A8(&D_80119870);
    if (temp_v0 != NULL) {
        temp_v1 = ((__typeof__(&D_80119870))((s8 *)&D_80119870 + 0x1268));
        M2C_FIELD(temp_v0, s32 *, 0x9EC) = 1;
        M2C_FIELD(temp_v1, s32 *, 4) = (s32) (M2C_FIELD(temp_v1, s32 *, 4) | 4);
        func_00248BA8((s32) temp_v0, 0x1B57, 2, 0);
    }
}
