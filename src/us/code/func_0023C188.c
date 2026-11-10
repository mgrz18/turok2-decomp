#include "common.h"
#include "structs/collision.h"
#include "structs/region.h"
#include "globals/scene.h"

#ifdef TARGET_PORT
extern CRegionInfo *CScene__GetRegionInfo(void *arg0, void *region);
#else
extern CRegionInfo *CScene__GetRegionInfo(void *arg0);
#endif

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__GetRegionInfo gets this
 * function's $a1 untouched, the region its two callers pass; the host takes and passes it. */
s32 Sweep_GetGroundBehavior(Sweep *sweep, void *region) {
#else
s32 Sweep_GetGroundBehavior(Sweep *sweep) {
#endif
    CRegionInfo *target;

#ifdef TARGET_PORT
    target = CScene__GetRegionInfo(&gScene, region);
#else
    target = CScene__GetRegionInfo(&gScene);
#endif
    if (target != NULL && (target->m_dwFlags & 0x4000)) {
        return 0;
    }
    return sweep->def->m_GroundBehavior;
}
