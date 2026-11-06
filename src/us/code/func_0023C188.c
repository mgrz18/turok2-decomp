#include "common.h"
#include "structs/collision.h"
#include "structs/region.h"
#include "globals/scene.h"

extern CRegionInfo *CScene__GetRegionInfo(void *arg0);

s32 Sweep_GetGroundBehavior(Sweep *sweep) {
    CRegionInfo *target;

    target = CScene__GetRegionInfo(&gScene);
    if (target != NULL && (target->m_dwFlags & 0x4000)) {
        return 0;
    }
    return sweep->def->m_GroundBehavior;
}
