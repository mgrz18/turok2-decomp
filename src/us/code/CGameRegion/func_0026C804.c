#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "structs/region.h"



void CGameRegion__SetFlag40000(s32 arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags |= 0x40000;
    }
}
