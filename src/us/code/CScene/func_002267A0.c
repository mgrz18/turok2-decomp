#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"


void func_002267A0(CScene *arg0, s32 arg1, s32 arg2, s32 arg3) {
    sptr temp_v0;
    sptr var_a0;

    if (arg1 != 0) {
        var_a0 = (sptr) arg0->m_pBitSections;
    } else {
        var_a0 = (sptr) arg0->m_pBitSections2;
    }
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(var_a0, 0), arg3), arg2);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    CISet__GetBlock(temp_v0, 1);
}
