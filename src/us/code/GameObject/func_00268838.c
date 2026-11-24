#include "common.h"
#include "protos/game_object.h"

/* GameObject_GetRegionFloorHeight, inlined here. */
static __inline__ f32 floorHeight(CInstanceHeader *obj, CVector3 *offset) {
    CGameRegion *node = obj->m_pCurrentRegion;
    f32 result;

    if (node == NULL) {
        result = obj->m_vPos.y;
    } else if (offset == NULL) {
        result = CGameRegion__GetHeight(node, obj->m_vPos.x, obj->m_vPos.z);
    } else {
        result = CGameRegion__GetHeight(node, obj->m_vPos.x + offset->x, obj->m_vPos.z + offset->z);
    }
    return result;
}

/* Whether the actor is on (or within 0.4 above) the floor. Round 28 joined the N64 view and the
 * port's host-offset one (round 11 C26) on CGameObjectInstance. */
s32 GameObject_IsNearFloor(CGameObjectInstance *actor) {
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x1000)) {
        return 1;
    }
    return actor->header.m_vPos.y - floorHeight(&actor->header, NULL) <= 0.4f;
}
