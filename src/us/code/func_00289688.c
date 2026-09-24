#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002759C4(M2C_UNK);                     /* extern */
M2C_UNK func_002888BC(s32, M2C_UNK, M2C_UNK);       /* extern */
M2C_UNK func_00416644(s32, M2C_UNK *, M2C_UNK, M2C_UNK *, s32); /* extern */
M2C_UNK func_0042AC0C(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_0042F514();                            /* extern */
extern M2C_UNK D_004392B0;
extern M2C_UNK D_800F5C78;
extern M2C_UNK D_800F7078;

void func_00289688(s32 arg0) {
    func_0042F514();
    func_002888BC(arg0, 0, 0);
    func_002759C4(0x10);
    func_0042AC0C(&D_800F7078, 0, 0);
    func_00416644(arg0 + 0x2317C, &D_004392B0, 0, &D_800F5C78, 0);
}
