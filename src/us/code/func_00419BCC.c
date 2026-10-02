#include "common.h"
#include "m2c_macros.h"

s32 func_002759C4(s32);
s32 func_00416454(s32, s32, s32);

M2C_UNK func_00275904(M2C_UNK);                     /* extern */
M2C_UNK func_0027598C(M2C_UNK);                     /* extern */
M2C_UNK func_00275A50();                            /* extern */
M2C_UNK func_00285CD4();                            /* extern */
extern s32 D_800C1F70;
extern s32 D_800C1F74;

void func_00419BCC(s32 arg0, s32 arg1, s32 arg2) {
    func_00275904(-1);
    func_0027598C(0);
    if (D_800C1F74 != 0) {
        func_00275A50();
    }
    func_002759C4(D_800C1F70);
    func_00285CD4();
    func_00416454(arg0, arg1, arg2);
}
