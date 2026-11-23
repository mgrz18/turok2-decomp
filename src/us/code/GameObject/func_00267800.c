#include "common.h"
#include "structs/game_object.h"


f32 CGameRegion__GetHeight(void *node, f32 x, f32 z);
f32 Platform_GetTopHeight(void *actor, f32 x, f32 z);
void GameObject_GetFrameDelta(CGameObjectInstance *actor, CVector3 *velocity);

/* GameObject_GetFloorHeightAt, inlined: floor height under (x, z) for an actor. */
static __inline__ f32 actorFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    CIntelligence *def;

    if (actor->header.m_Type == 1) {
        def = actor->header.m_pIntelligence;
        if (def->m_Type == 2) {
            switch (def->m_Platform.m_CollisionType) {
                case 0:
                    if (actor->header.m_pCurrentRegion != NULL) {
                        return CGameRegion__GetHeight(actor->header.m_pCurrentRegion, x, z);
                    }
                    return actor->header.m_vPos.y;
                case 1:
                case 2:
                    return Platform_GetTopHeight(actor, x, z);
            }
        }
    }
    return actor->header.m_vPos.y;
}

/* The platform an actor stands on, if it is still touching it. */
CGameObjectInstance *GameObject_GetPlatformBelow(CGameObjectInstance *actor) {
    CGameObjectInstance *platform;
    CVector3 velocity;
    f32 floor;
    f32 gap;

    platform = NULL;
    if (actor->header.m_Type == 1) {
        platform = actor->m_pInstanceBelow;
    }
    if (platform == NULL) {
        return NULL;
    }
    if (platform->header.m_pIntelligence->m_Type != 2) {
        return platform;
    }
    floor = actorFloor(platform, actor->header.m_vPos.x, actor->header.m_vPos.z);
    GameObject_GetFrameDelta(platform, &velocity);
    gap = actor->header.m_vPos.y - floor + velocity.y;
    if ((gap < 0.0f) ? (-gap <= 5.12f) : (gap <= 5.12f)) {
        return platform;
    }
    return NULL;
}
