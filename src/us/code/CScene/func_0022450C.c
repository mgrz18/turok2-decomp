#include "common.h"
#include "protos/iset.h"
#include "structs/world.h"


/* Tests a bit of the current level's second saved bitmap. */
s32 CScene__GetLevelBit(CScene *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_s0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg1;
    temp_s0 = arg0->m_nLevel;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(arg0->m_pBitSections2, 0), temp_s0), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    temp_a0 = CISet__GetBlock(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}
