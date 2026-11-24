#include "common.h"
#include "structs/collision.h"
#include "structs/region.h"
#include "globals/scene.h"

#include "protos/scene.h"

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__GetRegionInfo gets this
 * function's $a1 untouched, the region its two callers pass; the host takes and passes it. */
s32 Sweep_GetGroundBehavior(Sweep *sweep, void *region) {
    CRegionInfo *target;

    target = CScene__GetRegionInfo(&gScene, region);
    if (target != NULL && (target->m_dwFlags & 0x4000)) {
        return 0;
    }
    return sweep->def->m_GroundBehavior;
}
