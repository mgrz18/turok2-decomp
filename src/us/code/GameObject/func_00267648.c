#include "common.h"
#include "structs/game_object.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))


f32 CGameRegion__GetHeight(void *node, f32 x, f32 z);
f32 Platform_GetTopHeight(void *actor, f32 x, f32 z);

static __inline__ f32 nodeFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    void *node = actor->header.m_pCurrentRegion;
    f32 result;

    if (node == NULL) {
        result = actor->header.m_vPos.y;
    } else {
        result = CGameRegion__GetHeight(node, x, z);
    }
    return result;
}

/* GameObject_GetFloorHeightAt, inlined: floor height under (x, z) for an actor. */
static __inline__ f32 actorFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    CIntelligence *def;

    if (actor->header.m_Type == 1) {
        def = actor->header.m_pIntelligence;
        if (def->m_Type == 2) {
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

static __inline__ CGameObjectInstance *standOn(CGameObjectInstance *actor) {
    CGameObjectInstance *result = NULL;

    if (actor->header.m_Type == 1) {
        result = actor->m_pInstanceBelow;
    }
    return result;
}

/* Ground height under an actor: its own region floor, or what it stands on if higher. */
f32 GameObject_GetGroundHeight(CGameObjectInstance *actor) {
    f32 ground;

    if (actor->header.m_pCurrentRegion == NULL) {
        return actor->header.m_vPos.y;
    }
    ground = CGameRegion__GetHeight(actor->header.m_pCurrentRegion, actor->header.m_vPos.x, actor->header.m_vPos.z);
    if (standOn(actor) == NULL) {
        return ground;
    }
    return MAX(ground, actorFloor(standOn(actor), actor->header.m_vPos.x, actor->header.m_vPos.z));
}
