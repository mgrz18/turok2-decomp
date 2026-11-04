#include "common.h"

s32 func_002759C4(s32);                     /* extern */
s32 func_002888BC(s32, s32, s32);       /* extern */
s32 func_00416644(s32, s32 *, s32, s32 *, s32); /* extern */
s32 func_0042AC0C(s32 *, s32, s32); /* extern */
s32 func_0042F514();                            /* extern */
extern s32 D_004392B0;
extern s32 D_800F5C78;
extern s32 D_800F7078;

void func_00289688(s32 arg0) {
    func_0042F514();
    func_002888BC(arg0, 0, 0);
    func_002759C4(0x10);
    func_0042AC0C(&D_800F7078, 0, 0);
    func_00416644(arg0 + 0x2317C, &D_004392B0, 0, &D_800F5C78, 0);
}
