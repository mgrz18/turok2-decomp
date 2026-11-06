#include "common.h"
#include "structs/world.h"

s32 CISet__GetBlock(s32, s32);                        /* extern */

s32 CScene__GetLinkGroupCountPtr(CScene *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = CISet__GetBlock((s32)arg0->m_pBitSections, 1);
    CISet__GetBlock(temp_v0, 0);
    return CISet__GetBlock(temp_v0, 1) + arg1;
}
