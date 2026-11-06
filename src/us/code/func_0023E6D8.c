#include "common.h"
#include "structs/collision.h"

f32 GameObject_GetCollisionHeight(CGameObjectInstance *inst);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *inst);
f32 func_0026841C(CGameObjectInstance *inst);

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* Fill the bounds of the volume a sweep's instance moves through. */
void Sweep_GetVolumeBounds(Sweep *sweep, SweepBox *box) {
    CGameObjectInstance *inst;
    CCollisionInfo *def;
    f32 x;
    f32 z;

    inst = sweep->obj;
    def = sweep->def;
    box->radius = GameObject_GetCollisionRadius(inst);
    box->flag = def->m_dwFlags & 0x100000;
    box->bottom = func_0026841C(inst);
    box->top = box->bottom + GameObject_GetCollisionHeight(inst);
    box->startBottom = inst->header.m_vPos.y + box->bottom;
    box->startTop = inst->header.m_vPos.y + box->top;
    box->endBottom = sweep->end.y + box->bottom;
    box->endTop = sweep->end.y + box->top;
    x = inst->header.m_vPos.x;
    z = inst->header.m_vPos.z;
    box->minX = MIN(sweep->end.x, x) - box->radius;
    box->maxX = MAX(sweep->end.x, x) + box->radius;
    box->minZ = MIN(sweep->end.z, z) - box->radius;
    box->maxZ = MAX(sweep->end.z, z) + box->radius;
}
