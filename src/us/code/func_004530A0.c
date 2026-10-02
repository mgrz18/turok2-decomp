#include "common.h"
#include "m2c_macros.h"

s32 func_002759C4(s32);
s32 func_00416894(s32, s32, s32);

M2C_UNK func_00275904(M2C_UNK);                     /* extern */
M2C_UNK func_0027598C(M2C_UNK);                     /* extern */
s32 func_002759B4();                                /* extern */
M2C_UNK func_00275A74();                            /* extern */
M2C_UNK func_00275A98();                            /* extern */
M2C_UNK func_00285CC4();                            /* extern */
extern M2C_UNK D_004393D4;
extern s32 D_800C1F70;
extern s32 D_800C1F74;
extern s32 D_800C1F78;

s32 func_004530A0(s32 arg0, s32 arg1, s32 arg2) {
    func_00275904(0x12C);
    func_00275A98();
    D_800C1F70 = func_002759B4();
    func_002759C4(D_800C1F78);
    func_00275A74();
    func_0027598C(1);
    D_800C1F74 = 1;
    func_00285CC4();
    func_00416894(arg2, arg1, (s32) &D_004393D4);
    return 1;
}
