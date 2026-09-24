#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200500(s32, M2C_UNK, M2C_UNK);       /* extern */
M2C_UNK func_00200574(s32, void *);                 /* extern */
M2C_UNK func_00266C5C(s32, s32, M2C_UNK);           /* extern */
M2C_UNK func_0029B030(s32, s32, M2C_UNK);           /* extern */
M2C_UNK func_0029B060(s32, s32, M2C_UNK *, s32, s32, s32); /* extern */
M2C_UNK func_0029B950(M2C_UNK, s32, M2C_UNK);       /* extern */
M2C_UNK func_0029BB10(s32);                         /* extern */
extern s32 D_800B6E94;
extern M2C_UNK func_00201FBC;

void func_00201CC0(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 var_s0;
    s32 var_s1;
    void *temp_a1;

    func_0029B030(arg0 + 0x230, arg0 + 0x248, 0x200);
    temp_s0 = arg0 + 0xA48;
    func_0029B030(temp_s0, arg0 + 0xA60, 1);
    func_0029B950(8, temp_s0, 0x7D1);
    func_00200500(arg0 + 0x5068, 0x18, 0x1C);
    func_00200500(arg0 + 0x507C, 0x18, 0x1C);
    var_s1 = 0;
    var_s0 = 0xE68;
    do {
        temp_a1 = arg0 + var_s0;
        M2C_FIELD(temp_a1, s32 *, 0x14) = 0;
        func_00200574(arg0 + 0x5068, temp_a1);
        var_s1 += 1;
        var_s0 += 0x20;
    } while (var_s1 < 0x210);
    func_00266C5C(arg0 + 0xA68, arg1 & 0xFF, 0x400);
    func_0029B060(arg0, arg1, &func_00201FBC, arg0, arg0 + 0xE68, D_800B6E94);
    func_0029BB10(arg0);
}
