#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"


extern CCollisionInfo D_800F1E10;
extern CCollisionResults *D_800F1ADC;

s32 func_00241E30(Sweep *ctrl, CGameObjectInstance *actor);
s32 func_00242284(Sweep *ctrl, CGameObjectInstance *actor);
s32 func_00241FC8(Sweep *ctrl, CGameObjectInstance *actor);

void func_00242604(Sweep *ctrl, CGameObjectInstance *actor) {
    CPlatformIntelligence *info;
    CCollisionInfo *def;
    s32 ok;

    info = &actor->header.m_pIntelligence->m_Platform;
    def = ctrl->def;
    if (def != &D_800F1E10 && def->m_InstanceBehavior != 0) {
        switch (info->m_CollisionType) {
            case 0:
                ok = func_00241E30(ctrl, actor);
                break;
            case 1:
                ok = func_00242284(ctrl, actor);
                break;
            case 2:
                ok = func_00241FC8(ctrl, actor);
                break;
            default:
                ok = 0;
                break;
        }
        if (ok != 0) {
            ctrl->hit.kindOut = NULL;
            ctrl->hit.kind = 0;
            ctrl->hit.data = actor;
            ctrl->hit.posOut = &D_800F1ADC->m_vShapePos;
            ctrl->hit.regionOut = &D_800F1ADC->m_pShapeRegion;
            ctrl->hit.dataOut = (void **)&D_800F1ADC->m_pShape;
        }
    }
}
