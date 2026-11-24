#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "structs/game_object.h"
#include "structs/region.h"

/* The CRegionInfo of the actor's current region (0 without one). */
sptr GameObject_GetRegionInfo(CInstanceHeader *arg0) {
    CGameRegion *r = arg0->m_pCurrentRegion;
    return r != NULL ? (sptr) CScene__GetRegionInfo(&gScene, &r->m_nInfo) : 0;
}
