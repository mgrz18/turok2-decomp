#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"
#include "globals/scene.h"

extern CGameRegion *sCachedRegionInfoRegion;     /* region of the cached info */
extern CRegionInfo *sCachedRegionInfo; /* its cached info */

extern CRegionInfo *CScene__GetRegionInfo(void *table, CGameRegion *region);

/* Whether the segment from a to b crosses the height of the region's surface,
 * strictly between its ends; *t gets the crossing fraction.
 * The failure paths share the first `return 0` through a label: the ROM's
 * single `v0 = 0; j epilogue` block sits right after the region test, where
 * three separate returns would be cross-jumped into the last one instead. */
s32 CGameRegion__IntersectWater(CGameRegion *region, CVector3 *a, CVector3 *b, f32 *t) {
    CRegionInfo *info;
    f32 dy;
    f32 f;
    s32 result;

    if (region == NULL || !(region->m_wFlags & 1)) {
    fail:
        return 0;
    }
    if (region != sCachedRegionInfoRegion) {
        sCachedRegionInfo = CScene__GetRegionInfo(&gScene, region);
        sCachedRegionInfoRegion = region;
    }
    info = sCachedRegionInfo;
    if (info == NULL) {
        goto fail;
    }
    dy = b->y - a->y;
    if (dy == 0.0f) {
        goto fail;
    }
    result = 0;
    f = (info->m_WaterHeight - a->y) / dy;
    *t = f;
    if (f > 0.0f && f < 1.0f) {
        result = 1;
    }
    return result;
}
