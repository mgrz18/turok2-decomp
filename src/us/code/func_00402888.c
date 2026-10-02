#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);
s32 func_00402404(s32, s32, s32);
s32 func_00402A10(s32, s32);
s32 func_00403660(s32, s32, s32);

extern M2C_UNK D_00430CA4;
extern s32 D_80110048;
extern M2C_UNK func_0025E4B8;

void func_00402888(s32 arg0, void *arg1) {
    D_80110048 = 0x1D4D;
    M2C_FIELD(arg1, M2C_UNK **, 0x2C) = &D_00430CA4;
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xF0) = func_00402A10;
    M2C_FIELD(arg1, s32 (**)(s32, s32, s32), 0xF8) = func_00402404;
    M2C_FIELD(arg1, M2C_UNK **, 0x108) = &func_0025E4B8;
    M2C_FIELD(arg1, s32 (**)(s32, s32, s32), 0xF4) = func_00403660;
    M2C_FIELD(arg1, s32 *, 0xC) = -1;
    func_00243414(arg0, (s32) arg1, 0);
}
