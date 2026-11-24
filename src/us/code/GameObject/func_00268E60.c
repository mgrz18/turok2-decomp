#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "structs/game_object.h"
#include "structs/region.h"

s32 CGameObjectInstance__GetGeometryKind(void *arg);

/* The kind of floor an object stands on (footsteps, splashes): -1, 7 or 8 for the region flags
 * 0x4000, 0x400000 and 0x80 (second word), else the region's own kind; 1 while the AI's flag 0x400
 * is set; what the object below is made of while it rides one (AI flag 0x40000). */
s32 GameObject_GetFloorKind(CGameObjectInstance *obj) {
    CGameRegion *region = obj->header.m_pCurrentRegion;
    CRegionInfo *info;
    s32 result = 0;
    u32 objFlags;
    CGameObjectInstance *other;

    if (region != NULL && (info = CScene__GetRegionInfo(&gScene, &region->m_nInfo)) != NULL) {
        if (info->m_dwFlags & 0x4000) {
            result = -1;
        } else if (info->m_dwFlags & 0x400000) {
            result = 7;
        } else if (info->m_wFlags52 & 0x80) {
            result = 8;
        } else {
            result = (u8) info->unk_58;
        }
    }
    if (obj->header.m_Type == 1) {
        objFlags = obj->m_AI.m_dwFlags;
        if (objFlags & 0x400) {
            result = 1;
        }
        if (objFlags & 0x40000) {
            other = obj->m_pInstanceBelow;
            if (other != NULL && other->header.m_Type == obj->header.m_Type) {
                result = CGameObjectInstance__GetGeometryKind(other);
            }
        }
    }
    return result;
}
