#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "structs/region.h"

/* A 0x64-byte record of the world model sections (CScene__GetRegionInfo finds it from an id). */

s32 CGameRegion__FloodSetFlag4000Same(u16 *, u16);                  /* extern */

void CGameRegion__SetFlag4000(u16 *arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 != NULL) {
        temp_v0->m_dwFlags |= 0x4000;
    }
    if (arg0 != NULL) {
        CGameRegion__FloodSetFlag4000Same(arg0, *arg0);
    }
}
