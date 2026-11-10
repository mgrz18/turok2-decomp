#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

extern void func_00237FAC(sptr, CVector3, s32);

void func_00238824(s32 arg0, s32 arg1, s32 arg2, CVector3 arg3) {
    func_00237FAC(arg0, arg3, 1);
}
