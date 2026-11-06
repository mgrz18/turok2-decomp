#include "common.h"
#include "structs/world.h"

s32 CISet__GetBlock(s32, s32);                    /* extern */
s32 CISet__GetBlockSize(s32, s32);                /* extern */

void func_002267A0(CScene *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_a0;

    if (arg1 != 0) {
        var_a0 = (s32) arg0->m_pBitSections;
    } else {
        var_a0 = (s32) arg0->m_pBitSections2;
    }
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(var_a0, 0), arg3), arg2);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    CISet__GetBlock(temp_v0, 1);
}
