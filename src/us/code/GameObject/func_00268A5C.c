#include "common.h"
#include "structs/region.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"


f32 GameObject_GetWaterHeight(CInstanceHeader *arg0) {
    CGameRegion *temp_a1;
    CRegionInfo *temp_v0;

    temp_a1 = arg0->m_pCurrentRegion;
    if ((temp_a1 != NULL) && (temp_a1->m_wFlags & 1)) {
        temp_v0 = CScene__GetRegionInfo(&gScene, temp_a1);
        if (temp_v0 != NULL) {
            return temp_v0->m_WaterHeight;
        }
    }
    return 0.0f;
}
