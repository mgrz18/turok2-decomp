#include "common.h"
#include "structs/collision.h"
#include "globals/collision.h"


f32 func_00268460(CGameObjectInstance *inst);
void Collision_SweepRegionWalls(void *region);
void CGameRegion__ClearVisited(void *region);

/* Sweep an instance against the map, starting from its region. */
void Collision_SweepMapWalls(Sweep *sweep) {
    MapSweep ms;
    CCollisionInfo *def;
    CGameObjectInstance *inst;
    void *region;
    f32 lo;
    f32 hi;

    def = sweep->def;
    if (!(def->m_dwFlags & 2) || def->m_WallBehavior == 0 || func_00268460(sweep->obj) == 0.0f) {
        return;
    }
    inst = sweep->obj;
    region = inst->header.m_pCurrentRegion;
    if (region == NULL) {
        return;
    }
    if (inst->header.m_Type == 1 && !(inst->m_dwFlags & 0x300200)) {
        return;
    }
    sMapSweep = &ms;
    sMapSweep->radius = func_00268460(inst);
    sMapSweep->sweep = sweep;
    sMapSweep->normal.y = 0.0f;
    sMapSweep->wallPoint.y = 0.0f;
    if (sweep->inst->m_vPos.x < sweep->end.x) {
        lo = sweep->inst->m_vPos.x;
        hi = sweep->end.x;
    } else {
        lo = sweep->end.x;
        hi = sweep->inst->m_vPos.x;
    }
    sMapSweep->minX = lo - sMapSweep->radius;
    sMapSweep->maxX = hi + sMapSweep->radius;
    if (sweep->inst->m_vPos.z < sweep->end.z) {
        lo = sweep->inst->m_vPos.z;
        hi = sweep->end.z;
    } else {
        lo = sweep->end.z;
        hi = sweep->inst->m_vPos.z;
    }
    sMapSweep->minZ = lo - sMapSweep->radius;
    sMapSweep->maxZ = hi + sMapSweep->radius;
    Collision_SweepRegionWalls(region);
    CGameRegion__ClearVisited(region);
}
