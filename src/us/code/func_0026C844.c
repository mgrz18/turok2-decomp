#include "common.h"
#include "structs/region.h"

CRegionInfo *func_0022425C(void *, s32);             /* extern */
extern u8 D_800F7078[];

void func_0026C844(s32 arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = func_0022425C(D_800F7078, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags = (s32) (temp_v0->m_dwFlags & 0xFFFBFFFF);
    }
}
