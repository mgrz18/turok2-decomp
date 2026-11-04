#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/collision.h"

extern MapSweep *D_800F1AB0;
extern CCollisionResults *D_800F1ADC;
extern u8 D_800C894C;                   /* bottom of the stack */

f32 func_00298470(f32 x);               /* sqrtf */
s32 func_0023AB18(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 func_0023B38C(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside);
CGameRegion *func_0023B664(Sweep *mover, CGameRegion *region, s32 edge, CVector3 *pos);

/* Walk the regions the sweep's box overlaps, from region, sweeping the
 * sweep's circle against each wall edge (and the corners at its ends). */
void func_00240800(CGameRegion *region) {
    u8 cornerDone[3];
    s32 i;

    if ((u32) &region < (u32) &D_800C894C) {
        return;
    }
    i = 0;
    region->m_wFlags |= 0x8000;
    cornerDone[2] = 0;
    cornerDone[1] = 0;
    cornerDone[0] = 0;
    do {
        D_800F1AB0->next = region->m_pNeighbours[i];
        if (D_800F1AB0->next != NULL) {
            if (D_800F1AB0->next->m_wFlags & 0x8000) {
                continue;
            }
            if (region->m_wFlags & 0x20) {
                if (D_800F1AB0->next->m_pNeighbours[0] != region && D_800F1AB0->next->m_pNeighbours[1] != region &&
                    D_800F1AB0->next->m_pNeighbours[2] != region) {
                    continue;
                }
            } else {
                D_800F1AB0->next = func_0023B664(D_800F1AB0->sweep, region, i, &D_800F1AB0->sweep->inst->m_vPos);
            }
        }
        D_800F1AB0->a = region->m_pVertices[i];
        D_800F1AB0->b = region->m_pVertices[i == 2 ? 0 : i + 1];
        if (D_800F1AB0->a->x <= D_800F1AB0->minX && D_800F1AB0->b->x <= D_800F1AB0->minX) {
            continue;
        }
        if (D_800F1AB0->maxX <= D_800F1AB0->a->x && D_800F1AB0->maxX <= D_800F1AB0->b->x) {
            continue;
        }
        if (D_800F1AB0->a->z <= D_800F1AB0->minZ && D_800F1AB0->b->z <= D_800F1AB0->minZ) {
            continue;
        }
        if (D_800F1AB0->maxZ <= D_800F1AB0->a->z && D_800F1AB0->maxZ <= D_800F1AB0->b->z) {
            continue;
        }
        if (D_800F1AB0->next != NULL) {
            func_00240800(D_800F1AB0->next);
            continue;
        }
        D_800F1AB0->normal.x = D_800F1AB0->b->z - D_800F1AB0->a->z;
        D_800F1AB0->normal.z = D_800F1AB0->a->x - D_800F1AB0->b->x;
        D_800F1AB0->toStart.x = D_800F1AB0->sweep->inst->m_vPos.x - D_800F1AB0->a->x;
        D_800F1AB0->toStart.z = D_800F1AB0->sweep->inst->m_vPos.z - D_800F1AB0->a->z;
        D_800F1AB0->startSide = D_800F1AB0->normal.x * D_800F1AB0->toStart.x +
                                D_800F1AB0->normal.z * D_800F1AB0->toStart.z;
        if (!(D_800F1AB0->startSide >= 0.0f)) {
            continue;
        }
        D_800F1AB0->side.x = -D_800F1AB0->sweep->delta.z;
        D_800F1AB0->side.z = D_800F1AB0->sweep->delta.x;
        D_800F1AB0->edge.x = D_800F1AB0->b->x - D_800F1AB0->a->x;
        D_800F1AB0->edge.z = D_800F1AB0->b->z - D_800F1AB0->a->z;
        D_800F1AB0->across = D_800F1AB0->side.x * D_800F1AB0->edge.x + D_800F1AB0->side.z * D_800F1AB0->edge.z;
        if (!(D_800F1AB0->across < 0.0f)) {
            continue;
        }
        D_800F1AB0->hit = 0;
        if (!cornerDone[i]) {
            cornerDone[i] = 1;
            D_800F1AB0->hit |= func_0023B38C(D_800F1AB0->sweep, D_800F1AB0->a, D_800F1AB0->radius, 0, 0.0f, 0.0f, 1,
                                             D_800F1AB0->sweep->def->m_WallBehavior, 0.0f, 1);
        }
        if (!cornerDone[i == 2 ? 0 : i + 1]) {
            cornerDone[i == 2 ? 0 : i + 1] = 1;
            D_800F1AB0->hit |= func_0023B38C(D_800F1AB0->sweep, D_800F1AB0->b, D_800F1AB0->radius, 0, 0.0f, 0.0f, 1,
                                             D_800F1AB0->sweep->def->m_WallBehavior, 0.0f, 1);
        }
        D_800F1AB0->len = func_00298470(D_800F1AB0->normal.x * D_800F1AB0->normal.x +
                                        D_800F1AB0->normal.z * D_800F1AB0->normal.z);
        if (D_800F1AB0->len != 0.0f) {
            D_800F1AB0->len = D_800F1AB0->radius / D_800F1AB0->len;
            D_800F1AB0->wallPoint.x = D_800F1AB0->a->x + D_800F1AB0->normal.x * D_800F1AB0->len;
            D_800F1AB0->wallPoint.z = D_800F1AB0->a->z + D_800F1AB0->normal.z * D_800F1AB0->len;
            D_800F1AB0->toEnd.x = D_800F1AB0->sweep->end.x - D_800F1AB0->wallPoint.x;
            D_800F1AB0->toEnd.z = D_800F1AB0->sweep->end.z - D_800F1AB0->wallPoint.z;
            D_800F1AB0->endSide = D_800F1AB0->side.x * D_800F1AB0->toEnd.x + D_800F1AB0->side.z * D_800F1AB0->toEnd.z;
            D_800F1AB0->t = D_800F1AB0->endSide / D_800F1AB0->across;
            if (D_800F1AB0->t >= 0.0f && D_800F1AB0->t <= 1.0f &&
                !(D_800F1AB0->sweep->lastKind == 2 && D_800F1AB0->sweep->lastRadius == D_800F1AB0->radius &&
                  D_800F1AB0->sweep->lastContact.x == D_800F1AB0->a->x &&
                  D_800F1AB0->sweep->lastContact.y == D_800F1AB0->a->y &&
                  D_800F1AB0->sweep->lastContact.z == D_800F1AB0->a->z) &&
                !(D_800F1AB0->sweep->lastKind == 2 && D_800F1AB0->sweep->lastRadius == D_800F1AB0->radius &&
                  D_800F1AB0->sweep->lastContact.x == D_800F1AB0->b->x &&
                  D_800F1AB0->sweep->lastContact.y == D_800F1AB0->b->y &&
                  D_800F1AB0->sweep->lastContact.z == D_800F1AB0->b->z)) {
                D_800F1AB0->hit |= func_0023AB18(D_800F1AB0->sweep, &D_800F1AB0->normal, &D_800F1AB0->wallPoint, 0,
                                                 D_800F1AB0->sweep->def->m_WallBehavior, 0.0f, 1);
            }
        }
        if (D_800F1AB0->hit == 0) {
            continue;
        }
        D_800F1AB0->sweep->hit.posOut = &D_800F1ADC->m_vWallPos;
        D_800F1AB0->sweep->hit.regionOut = &D_800F1ADC->m_pWallRegion;
        D_800F1AB0->sweep->hit.kindOut = NULL;
        D_800F1AB0->sweep->hit.kind = 0;
        D_800F1AB0->sweep->hit.dataOut = NULL;
        D_800F1AB0->sweep->hit.data = NULL;
        D_800F1ADC->m_nWallEdge = i;
        if (D_800F1AB0->sweep->def->m_dwFlags & 0x20000000) {
            return;
        }
    } while (++i < 3);
}
