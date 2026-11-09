#include "structs/region.h"
#include "structs/particle.h"
#include "globals/scene.h"
#include "protos/scene.h"


/* Whether the particle is under water: below its region's water height (flag 4 when it has no region). */
s32 CParticle__IsUnderWater(CParticle *obj) {
    CRegionInfo *target;

    if (obj->header.m_pCurrentRegion != NULL) {
        if (!(obj->header.m_pCurrentRegion->m_wFlags & REGION_FLAG_WATER)) {
            return 0;
        }
        target = CScene__GetRegionInfo(&gScene, obj->header.m_pCurrentRegion);
        if (target != NULL) {
            goto found;
        }
    }
    return (obj->m_dwFlags >> 2) & 1;

found:
    if (target->m_dwFlags & 0x1000000) {
        return 0;
    }
    return obj->header.m_vPos.y < target->m_WaterHeight;
}
