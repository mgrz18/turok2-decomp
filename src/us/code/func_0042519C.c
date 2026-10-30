#include "common.h"

void func_00275A74(void);
void func_00275E1C(s32);
s32 func_00275F7C(void);
void func_00285304(void);
void func_00285410(s32);
void func_00416454(s32, s32, s32);
extern s32 D_80130A34;

s32 func_0042519C(s32 arg0, s32 arg1, s32 arg2) {
    s32 ret;

    func_00275A74();
    func_00275E1C(func_00275F7C());
    if (D_80130A34 == 0) {
        func_00416454(arg0, arg1, arg2);
        ret = 1;
    } else {
        func_00285304();
        func_00285410(1);
        ret = 1;
    }
    return ret;
}
