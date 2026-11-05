#include "common.h"
#include "structs/collision.h"
#include "structs/region.h"

extern u8 D_800F7078[];
extern CRegionInfo *func_0022425C(void *arg0);

s32 func_0023C188(Sweep *sweep) {
    CRegionInfo *target;

    target = func_0022425C(D_800F7078);
    if (target != NULL && (target->m_dwFlags & 0x4000)) {
        return 0;
    }
    return sweep->def->m_GroundBehavior;
}
