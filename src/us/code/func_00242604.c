#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"


extern CCollisionInfo D_800F1E10;
extern CCollisionResults *gCollisionResultsP;

s32 Platform_CollideGround(Sweep *ctrl, CGameObjectInstance *actor);
s32 Platform_CollideCylinder(Sweep *ctrl, CGameObjectInstance *actor);
s32 Platform_CollideBox(Sweep *ctrl, CGameObjectInstance *actor);

void Platform_Collide(Sweep *ctrl, CGameObjectInstance *actor) {
    CPlatformIntelligence *info;
    CCollisionInfo *def;
    s32 ok;

    info = &actor->header.m_pIntelligence->m_Platform;
    def = ctrl->def;
    if (def != &D_800F1E10 && def->m_InstanceBehavior != 0) {
        switch (info->m_CollisionType) {
            case 0:
                ok = Platform_CollideGround(ctrl, actor);
                break;
            case 1:
                ok = Platform_CollideCylinder(ctrl, actor);
                break;
            case 2:
                ok = Platform_CollideBox(ctrl, actor);
                break;
            default:
                ok = 0;
                break;
        }
        if (ok != 0) {
            ctrl->hit.kindOut = NULL;
            ctrl->hit.kind = 0;
            ctrl->hit.data = actor;
            ctrl->hit.posOut = &gCollisionResultsP->m_vShapePos;
            ctrl->hit.regionOut = &gCollisionResultsP->m_pShapeRegion;
            ctrl->hit.dataOut = (void **)&gCollisionResultsP->m_pShape;
        }
    }
}
