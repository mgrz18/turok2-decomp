#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/collision.h"
#include "globals/collision.h"

extern u8 D_800C894C;                   /* bottom of the stack */

f32 func_00298470(f32 x);               /* sqrtf */
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 Sweep_HitCylinder(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside);
CGameRegion *Sweep_CrossRegionEdge(Sweep *mover, CGameRegion *region, s32 edge, CVector3 *pos);

/* Walk the regions the sweep's box overlaps, from region, sweeping the
 * sweep's circle against each wall edge (and the corners at its ends). */
void Collision_SweepRegionWalls(CGameRegion *region) {
    u8 cornerDone[3];
    s32 i;

    if ((uptr) &region < (uptr) &D_800C894C) {
        return;
    }
    i = 0;
    region->m_wFlags |= 0x8000;
    cornerDone[2] = 0;
    cornerDone[1] = 0;
    cornerDone[0] = 0;
    do {
        sMapSweep->next = region->m_pNeighbours[i];
        if (sMapSweep->next != NULL) {
            if (sMapSweep->next->m_wFlags & 0x8000) {
                continue;
            }
            if (region->m_wFlags & 0x20) {
                if (sMapSweep->next->m_pNeighbours[0] != region && sMapSweep->next->m_pNeighbours[1] != region &&
                    sMapSweep->next->m_pNeighbours[2] != region) {
                    continue;
                }
            } else {
                sMapSweep->next = Sweep_CrossRegionEdge(sMapSweep->sweep, region, i, &sMapSweep->sweep->inst->m_vPos);
            }
        }
        sMapSweep->a = region->m_pVertices[i];
        sMapSweep->b = region->m_pVertices[i == 2 ? 0 : i + 1];
        if (sMapSweep->a->x <= sMapSweep->minX && sMapSweep->b->x <= sMapSweep->minX) {
            continue;
        }
        if (sMapSweep->maxX <= sMapSweep->a->x && sMapSweep->maxX <= sMapSweep->b->x) {
            continue;
        }
        if (sMapSweep->a->z <= sMapSweep->minZ && sMapSweep->b->z <= sMapSweep->minZ) {
            continue;
        }
        if (sMapSweep->maxZ <= sMapSweep->a->z && sMapSweep->maxZ <= sMapSweep->b->z) {
            continue;
        }
        if (sMapSweep->next != NULL) {
            Collision_SweepRegionWalls(sMapSweep->next);
            continue;
        }
        sMapSweep->normal.x = sMapSweep->b->z - sMapSweep->a->z;
        sMapSweep->normal.z = sMapSweep->a->x - sMapSweep->b->x;
        sMapSweep->toStart.x = sMapSweep->sweep->inst->m_vPos.x - sMapSweep->a->x;
        sMapSweep->toStart.z = sMapSweep->sweep->inst->m_vPos.z - sMapSweep->a->z;
        sMapSweep->startSide = sMapSweep->normal.x * sMapSweep->toStart.x +
                                sMapSweep->normal.z * sMapSweep->toStart.z;
        if (!(sMapSweep->startSide >= 0.0f)) {
            continue;
        }
        sMapSweep->side.x = -sMapSweep->sweep->delta.z;
        sMapSweep->side.z = sMapSweep->sweep->delta.x;
        sMapSweep->edge.x = sMapSweep->b->x - sMapSweep->a->x;
        sMapSweep->edge.z = sMapSweep->b->z - sMapSweep->a->z;
        sMapSweep->across = sMapSweep->side.x * sMapSweep->edge.x + sMapSweep->side.z * sMapSweep->edge.z;
        if (!(sMapSweep->across < 0.0f)) {
            continue;
        }
        sMapSweep->hit = 0;
        if (!cornerDone[i]) {
            cornerDone[i] = 1;
            sMapSweep->hit |= Sweep_HitCylinder(sMapSweep->sweep, sMapSweep->a, sMapSweep->radius, 0, 0.0f, 0.0f, 1,
                                             sMapSweep->sweep->def->m_WallBehavior, 0.0f, 1);
        }
        if (!cornerDone[i == 2 ? 0 : i + 1]) {
            cornerDone[i == 2 ? 0 : i + 1] = 1;
            sMapSweep->hit |= Sweep_HitCylinder(sMapSweep->sweep, sMapSweep->b, sMapSweep->radius, 0, 0.0f, 0.0f, 1,
                                             sMapSweep->sweep->def->m_WallBehavior, 0.0f, 1);
        }
        sMapSweep->len = func_00298470(sMapSweep->normal.x * sMapSweep->normal.x +
                                        sMapSweep->normal.z * sMapSweep->normal.z);
        if (sMapSweep->len != 0.0f) {
            sMapSweep->len = sMapSweep->radius / sMapSweep->len;
            sMapSweep->wallPoint.x = sMapSweep->a->x + sMapSweep->normal.x * sMapSweep->len;
            sMapSweep->wallPoint.z = sMapSweep->a->z + sMapSweep->normal.z * sMapSweep->len;
            sMapSweep->toEnd.x = sMapSweep->sweep->end.x - sMapSweep->wallPoint.x;
            sMapSweep->toEnd.z = sMapSweep->sweep->end.z - sMapSweep->wallPoint.z;
            sMapSweep->endSide = sMapSweep->side.x * sMapSweep->toEnd.x + sMapSweep->side.z * sMapSweep->toEnd.z;
            sMapSweep->t = sMapSweep->endSide / sMapSweep->across;
            if (sMapSweep->t >= 0.0f && sMapSweep->t <= 1.0f &&
                !(sMapSweep->sweep->lastKind == 2 && sMapSweep->sweep->lastRadius == sMapSweep->radius &&
                  sMapSweep->sweep->lastContact.x == sMapSweep->a->x &&
                  sMapSweep->sweep->lastContact.y == sMapSweep->a->y &&
                  sMapSweep->sweep->lastContact.z == sMapSweep->a->z) &&
                !(sMapSweep->sweep->lastKind == 2 && sMapSweep->sweep->lastRadius == sMapSweep->radius &&
                  sMapSweep->sweep->lastContact.x == sMapSweep->b->x &&
                  sMapSweep->sweep->lastContact.y == sMapSweep->b->y &&
                  sMapSweep->sweep->lastContact.z == sMapSweep->b->z)) {
                sMapSweep->hit |= Sweep_HitPlane(sMapSweep->sweep, &sMapSweep->normal, &sMapSweep->wallPoint, 0,
                                                 sMapSweep->sweep->def->m_WallBehavior, 0.0f, 1);
            }
        }
        if (sMapSweep->hit == 0) {
            continue;
        }
        sMapSweep->sweep->hit.posOut = &gCollisionResultsP->m_vWallPos;
        sMapSweep->sweep->hit.regionOut = &gCollisionResultsP->m_pWallRegion;
        sMapSweep->sweep->hit.kindOut = NULL;
        sMapSweep->sweep->hit.kind = 0;
        sMapSweep->sweep->hit.dataOut = NULL;
        sMapSweep->sweep->hit.data = NULL;
        gCollisionResultsP->m_nWallEdge = i;
        if (sMapSweep->sweep->def->m_dwFlags & 0x20000000) {
            return;
        }
    } while (++i < 3);
}
