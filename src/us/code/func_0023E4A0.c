#include "common.h"
#include "structs/world.h"
#include "structs/collision.h"

extern CWorld D_800F7078;
extern s32 D_800F7964;
extern CCollisionInfo D_800F1C60;

f32 func_002671B4(CGameObjectInstance *inst);
f32 func_002672C8(CGameObjectInstance *inst);
f32 func_0026841C(CGameObjectInstance *inst);
void func_0023DF70(Sweep *sweep, SweepBox *box, CGameObjectInstance *other);

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* Sweep an instance's volume against every drawn instance it may hit. */
void func_0023E4A0(Sweep *sweep) {
    SweepBox box;
    SweepBox *b;
    CGameObjectInstance *inst;
    CCollisionInfo *def;
    CGameObjectInstance *other;
    CWorld *world;
    s32 count;
    f32 x;
    f32 z;
    s32 i;

    def = sweep->def;
    if (def->m_InstanceBehavior == 0 || (def->m_dwFlags & 0x40000000) || def == &D_800F1C60) {
        return;
    }
    inst = sweep->obj;
    count = D_800F7964; /* D_800F7078.drawCount */
    b = &box;
    box.radius = func_002672C8(inst);
    b->flag = def->m_dwFlags & 0x100000;
    b->bottom = func_0026841C(inst);
    b->top = b->bottom + func_002671B4(inst);
    b->startBottom = inst->header.m_vPos.y + b->bottom;
    b->startTop = inst->header.m_vPos.y + b->top;
    b->endBottom = sweep->end.y + b->bottom;
    b->endTop = sweep->end.y + b->top;
    x = inst->header.m_vPos.x;
    z = inst->header.m_vPos.z;
    b->minX = MIN(sweep->end.x, x) - box.radius;
    b->maxX = MAX(sweep->end.x, x) + box.radius;
    b->minZ = MIN(sweep->end.z, z) - box.radius;
    b->maxZ = MAX(sweep->end.z, z) + box.radius;
    world = &D_800F7078;
    for (i = 0; i < count; i++) {
        other = (CGameObjectInstance *)world->m_pDrawActors[i];
        if (other == inst || def->m_InstanceBehavior == 0) {
            continue;
        }
        switch (other->header.m_Type) {
            case 1:
                if ((def->m_dwFlags & 1) && other->m_AI.m_Health == 0) {
                    continue;
                }
                break;
            case 2:
                break;
            case 3:
                if (!(def->m_dwFlags & 0x20)) {
                    continue;
                }
                break;
        }
        func_0023DF70(sweep, &box, other);
    }
}
