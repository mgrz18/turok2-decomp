#include "common.h"
#include "globals/scene.h"
#include "structs/region.h"

CRegionInfo *CScene__GetRegionInfo(CScene *, s32);                /* extern */

void func_0026C724(s32 arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags = temp_v0->m_dwFlags | 0x80000;
    }
}
