#include "common.h"
#include "m2c_macros.h"

s32 func_00412438(M2C_UNK);                         /* extern */
M2C_UNK func_0042C418(M2C_UNK *);                   /* extern */
M2C_UNK func_0042C46C(M2C_UNK *);                   /* extern */
M2C_UNK func_0042C700(M2C_UNK *, s32, M2C_UNK);     /* extern */
M2C_UNK func_0042E5AC(s32, M2C_UNK, M2C_UNK);       /* extern */
M2C_UNK func_0042FBA4(s32, M2C_UNK);                /* extern */
extern M2C_UNK D_800F7078;

void func_00288DD0(s32 arg0, M2C_UNK arg1) {
    func_0042FBA4(arg0 + 0x22C00, 1);
    func_0042E5AC(arg0 + 0x22BC0, 1, 0);
    func_0042C700(&D_800F7078, ~func_00412438(arg1), 0);
    func_0042C418(&D_800F7078);
    func_0042C46C(&D_800F7078);
}
