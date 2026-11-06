#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"
#include "structs/collision.h"
#include "globals/scene.h"


/* A region corner: its floor point and the ceiling height above it. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 ceiling;
} Corner;

#define REGION_CORNER(r, i) ((Corner *)(r)->m_pVertices[i])

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 level;
    /* 0x24 */ u8 pad24[0x44 - 0x24];
    /* 0x44 */ u32 flags;
    /* 0x48 */ u8 pad48[0x52 - 0x48];
    /* 0x52 */ u16 flags52;
} Water;

extern CCollisionResults *gCollisionResultsP;

void memset(void *p, s32 c, s32 n);                 /* memset */
s32 Sweep_GetGroundBehavior(Sweep *sweep, CGameRegion *region);
void CGameRegion__GetNormalRaw(CVector3 *normal, CGameRegion *region);        /* floor plane */
void CGameRegion__GetCeilingNormalRaw(CVector3 *normal, CGameRegion *region);        /* ceiling plane */
s32 CGameRegion__IsSteep(CGameRegion *region);
Water *CScene__GetRegionInfo(u8 *scene, CGameRegion *region);
f32 GameObject_GetCollisionHeight(CGameObjectInstance *inst);                       /* height */
void CVector3__Subtract(CVector3 *out, void *a, CVector3 *b);        /* out = a - b */
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *base, CVector3 *dir);   /* out = base + t * dir */
CGameRegion *Sweep_CrossRegionEdge(Sweep *sweep, CGameRegion *region, s32 edge, CVector3 *point);
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, void *point, s32 arg3, s32 arg4, f32 arg5, s32 arg6);

static __inline__ f32 Dot(CVector3 *a, CVector3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* Walk a sweep through the map's regions: in each, find where it leaves
 * through the floor, the ceiling, the water surface or an edge, record the
 * nearest hit, or cross the edge into the next region (100 at most). */
void Collision_SweepRegions(Sweep *sweep) {
    CVector3 edgeN;
    CVector3 floorN;
    CVector3 ceilN;
    CVector3 up = { 0.0f, 1.0f, 0.0f };
    CVector3 down = { 0.0f, -1.0f, 0.0f };
    CVector3 surface;
    CVector3 rel;
    CVector3 ceilPt;
    CVector3 at;
    CCollisionInfo *def;
    CVector3 *pos;
    CGameRegion **regionp;
    CGameRegion *next;
    Water *water;
    f32 best;
    f32 floorT;
    f32 ceilT;
    f32 waterT;
    f32 d;
    f32 t;
    f32 num;
    f32 ex;
    f32 ez;
    f32 arg;
    Corner *a;
    Corner *b;
    s32 count;
    s32 edge;
    s32 i;
    s32 kind;
    s32 v;

    memset(&surface, 0, sizeof(surface));
    if ((sweep->def->m_dwFlags & 0x1000000) || sweep->inst->m_pCurrentRegion == NULL || sweep->def->m_WallBehavior == 0) {
        return;
    }
    def = sweep->def;
    pos = &sweep->inst->m_vPos;
    regionp = &sweep->inst->m_pCurrentRegion;
    count = 0;
    best = -10000.0f;
    do {
        floorT = 1.0f;
        if (sweep->checkRegionInfo != 0) {
            v = Sweep_GetGroundBehavior(sweep, sweep->inst->m_pCurrentRegion);
        } else {
            v = sweep->def->m_GroundBehavior;
        }
        if (sweep->onGround == 0 && v != 0) {
            CGameRegion__GetNormalRaw(&floorN, *regionp);
            d = floorN.x * sweep->delta.x + floorN.y * sweep->delta.y + floorN.z * sweep->delta.z;
            if (d < 0.0f) {
                CVector3__Subtract(&rel, (*regionp)->m_pVertices[0], pos);
                num = Dot(&floorN, &rel);
                floorT = num / d;
                if (floorT < best) {
                    floorT = 1.0f;
                }
            }
        }
        ceilT = 1.0f;
        if (sweep->def->m_GroundBehavior != 0 && ((*regionp)->m_wFlags & 0x40)) {
            CGameRegion__GetCeilingNormalRaw(&ceilN, *regionp);
            d = ceilN.x * sweep->delta.x + ceilN.y * sweep->delta.y + ceilN.z * sweep->delta.z;
            if (d < 0.0f) {
                ceilPt.x = (*regionp)->m_pVertices[0]->x;
                ceilPt.y = REGION_CORNER(*regionp, 0)->ceiling;
                if (sweep->def->m_dwFlags & 0x400000) {
                    ceilPt.y -= GameObject_GetCollisionHeight(sweep->obj);
                    if (sweep->def->m_dwFlags & 0x80) {
                        ceilPt.y += 44.032f;
                    }
                }
                ceilPt.z = (*regionp)->m_pVertices[0]->z;
                CVector3__Subtract(&rel, &ceilPt, pos);
                num = Dot(&ceilN, &rel);
                ceilT = num / d;
                if (ceilT < best) {
                    ceilT = 1.0f;
                }
            }
        }
        waterT = 1.0f;
        water = CScene__GetRegionInfo(&gScene, *regionp);
        if (((*regionp)->m_wFlags & 1) && (sweep->def->m_dwFlags & 8) && gCollisionResultsP->m_pWaterRegion == 0 && water != NULL
            && sweep->delta.y != 0.0f) {
            waterT = (water->level - pos->y) / sweep->delta.y;
            if (waterT < best) {
                waterT = 1.0f;
            }
        }
        best = sweep->hit.fraction;
        edge = 0;
        i = 0;
        do {
            a = REGION_CORNER(*regionp, i);
            b = REGION_CORNER(*regionp, (i != 2) ? i + 1 : 0);
            ex = b->z - a->z;
            ez = a->x - b->x;
            d = ex * sweep->delta.x + ez * sweep->delta.z;
            if (d < 0.0f) {
                t = (ex * (a->x - pos->x) + ez * (a->z - pos->z)) / d;
                if (t < best
                    && (sweep->lastKind != 1 || sweep->lastContact.x != ex || sweep->lastContact.y != 0.0f
                        || sweep->lastContact.z != ez)) {
                    best = t;
                    edge = i;
                    edgeN.x = ex;
                    edgeN.z = ez;
                }
            }
            i++;
        } while (i < 3);
        if (waterT <= best && waterT < sweep->hit.fraction) {
            surface.y = water->level;
            if (Sweep_HitPlane(sweep, (pos->y >= water->level) ? &up : &down, &surface, 1, 7, 0.0f, 1)) {
                if (water->flags & 0x400000) {
                    kind = 7;
                } else if (water->flags52 & 0x80) {
                    kind = 8;
                } else {
                    kind = 1;
                }
                sweep->hit.kind = kind;
                sweep->hit.dataOut = 0;
                sweep->hit.data = 0;
                sweep->hit.posOut = &gCollisionResultsP->m_vWaterPos;
                sweep->hit.regionOut = &gCollisionResultsP->m_pWaterRegion;
                sweep->hit.kindOut = &gCollisionResultsP->m_nWaterKind;
            }
        }
        if (floorT <= best && floorT < sweep->hit.fraction) {
            if (CGameRegion__IsSteep(*regionp) == 0) {
                arg = sweep->def->m_GroundFriction;
            } else {
                arg = 0.0f;
            }
            if (Sweep_HitPlane(sweep, &floorN, (*regionp)->m_pVertices[0], 0, sweep->def->m_GroundBehavior, arg, 1)) {
                sweep->hit.kind = 1;
                sweep->hit.dataOut = 0;
                sweep->hit.data = 0;
                sweep->hit.posOut = &gCollisionResultsP->m_vFloorPos;
                sweep->hit.regionOut = &gCollisionResultsP->m_pFloorRegion;
                sweep->hit.kindOut = &gCollisionResultsP->m_nFloorKind;
                if (def->m_dwFlags & 0x20000000) {
                    return;
                }
            }
        }
        if (ceilT <= best && ceilT < sweep->hit.fraction) {
            if (Sweep_HitPlane(sweep, &ceilN, &ceilPt, 0, sweep->def->m_WallBehavior, 0.0f, 1)) {
                sweep->hit.kind = 2;
                sweep->hit.dataOut = 0;
                sweep->hit.data = 0;
                sweep->hit.posOut = &gCollisionResultsP->m_vFloorPos;
                sweep->hit.regionOut = &gCollisionResultsP->m_pFloorRegion;
                sweep->hit.kindOut = &gCollisionResultsP->m_nFloorKind;
                if (def->m_dwFlags & 0x20000000) {
                    return;
                }
            }
        }
        if (!(best < sweep->hit.fraction)) {
            return;
        }
        CVector3__Lerp(&at, best, pos, &sweep->end);
        next = Sweep_CrossRegionEdge(sweep, *regionp, edge, &at);
        if (next == NULL || sweep->onGround != 0 || ((sweep->def->m_dwFlags & 0x8000) && (next->m_wFlags & 8))) {
            edgeN.y = 0.0f;
            if (Sweep_HitPlane(sweep, &edgeN, (*regionp)->m_pVertices[edge], 0, (next != NULL) ? 7 : sweep->def->m_WallBehavior, 0.0f, 1)) {
                if (next != NULL) {
                    *regionp = next;
                    sweep->hit.posOut = NULL;
                    sweep->hit.regionOut = NULL;
                    sweep->hit.kindOut = NULL;
                    sweep->hit.kind = 0;
                    sweep->hit.dataOut = 0;
                    sweep->hit.data = 0;
                    return;
                }
                sweep->hit.kindOut = NULL;
                sweep->hit.kind = 0;
                sweep->hit.dataOut = 0;
                sweep->hit.data = 0;
                sweep->hit.posOut = &gCollisionResultsP->m_vWallPos;
                sweep->hit.regionOut = &gCollisionResultsP->m_pWallRegion;
                gCollisionResultsP->m_nWallEdge = edge;
            }
            return;
        }
        count++;
        *regionp = next;
    } while (count < 100);
}
