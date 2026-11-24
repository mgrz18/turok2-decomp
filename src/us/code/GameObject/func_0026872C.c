#include "common.h"
#include "protos/game_object.h"

/* The floor height under an object (offset by offset in x and z when given): its region's, or
 * its own height when it has no region. */
f32 GameObject_GetRegionFloorHeight(CInstanceHeader *obj, CVector3 *offset) {
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
