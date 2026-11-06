#include "structs/region.h"
#include "structs/particle.h"
#include "globals/scene.h"

extern CRegionInfo *CScene__GetRegionInfo(void *arg0, CGameRegion *region);

/* Whether the particle is at or below the water height of its region's info flagged 0x1000000. */
s32 func_00232684(CParticle *obj) {
    CRegionInfo *target;

    if (obj->header.m_pCurrentRegion == NULL) {
        return 0;
    }
    if (!(obj->header.m_pCurrentRegion->m_wFlags & REGION_FLAG_WATER)) {
        return 0;
    }
    target = CScene__GetRegionInfo(&gScene, obj->header.m_pCurrentRegion);
    if (target == NULL) {
        return 0;
    }
    if (!(target->m_dwFlags & 0x1000000)) {
        return 0;
    }
    return obj->header.m_vPos.y <= target->m_WaterHeight;
}
