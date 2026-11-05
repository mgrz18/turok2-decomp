#include "common.h"
#include "structs/world.h"

s32 func_002017D4(s32, s32);                        /* extern */

s32 func_00226C7C(CWorld *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = func_002017D4((s32)arg0->m_pBitSections, 1);
    func_002017D4(temp_v0, 0);
    return func_002017D4(temp_v0, 1) + arg1;
}
