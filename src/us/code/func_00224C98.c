#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"


void CScene__DecLinkGroupCount(CScene *arg0, s32 arg1) {
    s32 temp_v0;
    u8 *temp_v0_2;

    temp_v0 = CISet__GetBlock((s32) arg0->m_pBitSections, 1);
    CISet__GetBlock(temp_v0, 0);
    temp_v0_2 = CISet__GetBlock(temp_v0, 1) + arg1;
    *temp_v0_2 -= 1;
}
