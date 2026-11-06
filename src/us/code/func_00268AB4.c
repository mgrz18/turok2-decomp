/* Cut fixed in versions/cuts.us.txt: GameObject_GetViewWaterHeight really is 0x268AB4-0x268B50 (0x9C bytes): splat split off its epilogue (jr ra; addiu sp) as func_00268B48. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/player.h"
#include "structs/camera.h"

extern CRegionInfo *CScene__GetRegionInfo(void *table, CGameRegion *region);
extern u8 gScene[];

/* Height of the actor's region (0 if it has none), relative to the owner's
 * height for a kind-1 actor flagged 0x300000. */
f32 GameObject_GetViewWaterHeight(CGameObjectInstance *actor) {
    CGameRegion *region;
    CRegionInfo *info;
    f32 h;

    region = actor->header.m_pCurrentRegion;
    if (region == NULL || !(region->m_wFlags & 1)
        || (info = CScene__GetRegionInfo(gScene, region)) == NULL) {
        return 0.0f;
    }
    h = info->m_WaterHeight;
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        h -= ((CPlayer *)actor->m_AI.m_pParent)->m_pCamera->m_vTCorners[0].y;
    }
    return h;
}
