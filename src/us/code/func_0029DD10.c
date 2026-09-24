#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0029B030(M2C_UNK *, M2C_UNK *, M2C_UNK); /* extern */
M2C_UNK func_0029B820(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
extern s32 D_800B8990;
extern M2C_UNK D_80124DB0;
extern M2C_UNK D_80124DB8;

void func_0029DD10(void) {
    D_800B8990 = 1;
    func_0029B030(&D_80124DB8, &D_80124DB0, 1);
    func_0029B820(&D_80124DB8, 0, 0);
}
