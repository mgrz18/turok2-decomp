#include "common.h"
#include "structs/world.h"

s32 func_002017D4(s32, s32);                    /* extern */
s32 func_00201818(s32, s32);                /* extern */

void func_002267A0(CWorld *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_a0;

    if (arg1 != 0) {
        var_a0 = (s32) arg0->m_pBitSections;
    } else {
        var_a0 = (s32) arg0->m_pBitSections2;
    }
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(var_a0, 0), arg3), arg2);
    func_002017D4(temp_v0, 0);
    func_00201818(temp_v0, 1);
    func_002017D4(temp_v0, 1);
}
