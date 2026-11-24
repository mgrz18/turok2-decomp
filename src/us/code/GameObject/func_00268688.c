#include "common.h"
#include "protos/game_object.h"

static __inline__ f32 nodeFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    CGameRegion *node = actor->header.m_pCurrentRegion;
    f32 result;

    if (node == NULL) {
        result = actor->header.m_vPos.y;
    } else {
        result = CGameRegion__GetHeight(node, x, z);
    }
    return result;
}

/* Floor height under (x, z) for the actor: a platform's top for the platforms that collide by
 * shape (collision types 1 and 2), its region's floor for type 0, its own height otherwise. */
f32 GameObject_GetFloorHeightAt(CGameObjectInstance *actor, f32 x, f32 z) {
    CIntelligence *def;

    if (actor->header.m_Type == 1) {
        def = actor->header.m_pIntelligence;
        if (def->m_Type == INTELLIGENCE_PLATFORM) {
            switch (def->m_Platform.m_CollisionType) {
                case 0:
                    return nodeFloor(actor, x, z);
                case 1:
                case 2:
                    return Platform_GetTopHeight(actor, x, z);
            }
        }
    }
    return actor->header.m_vPos.y;
}
