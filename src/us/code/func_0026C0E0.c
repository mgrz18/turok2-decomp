#include "common.h"
#include "globals/scene.h"
#include "structs/region.h"

CRegionInfo *CScene__GetRegionInfo(CScene *, s32);                /* extern */

u8 func_0026C0E0(s32 arg0) {
    CRegionInfo *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 == NULL) {
        return 0U;
    }
    return temp_v0->unk_58;
}
