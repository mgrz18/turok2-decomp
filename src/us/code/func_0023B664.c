#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/collision.h"

extern s32 D_800F7078[];

CRegionInfo *func_0022425C(s32 *list, CGameRegion *region);
s32 func_00267EA8(CGameObjectInstance *obj);
s32 func_00267EE8(CGameObjectInstance *obj);
s32 func_00267F10(CGameObjectInstance *obj);
s32 func_00267F64(CGameObjectInstance *obj);
s32 func_00267FA8(CGameObjectInstance *obj);
s32 func_00267FD0(CGameObjectInstance *obj);
s32 func_00268014(CGameObjectInstance *obj);
s32 func_0026803C(CGameObjectInstance *obj);
s32 func_00268080(CGameObjectInstance *obj);
s32 func_002680A8(CGameObjectInstance *obj);
s32 func_002680D0(CGameObjectInstance *obj);
s32 func_002680F8(CGameObjectInstance *obj);
s32 func_00268120(CGameObjectInstance *obj);
s32 func_0026AFC0(CGameRegion *region);
s32 func_0026BB9C(CGameRegion *region, f32 x, f32 z);
CGameRegion *func_0026B4A4(CGameRegion *region, CVector3 pos);
f32 func_00269C3C(CGameRegion *region, f32 x, f32 z);
f32 func_0026BE60(CGameRegion *region, f32 x, f32 z);
f32 func_0026C240(CGameRegion *region, s32 edge);
void func_00210EF0(f32 *angle);
f32 func_00210E64(f32 a, f32 b);

/* Whether the region's water is only a surface (not swum in). */
static __inline__ s32 isShallow(CRegionInfo *info) {
    return info->m_dwFlags & 0x20000000;
}

/* Whether the sweep's instance may go from region `from` across its edge to
 * the neighbouring region: returns that region (or the one found under pos
 * when the link is one-way), or NULL. */
CGameRegion *func_0023B664(Sweep *sweep, CGameRegion *from, s32 edge, CVector3 *pos) {
    CVector3 p;
    f32 angle;
    CGameRegion *to;
    CGameObjectInstance *obj;
    CCollisionInfo *def;
    CRegionInfo *fromInfo;
    CRegionInfo *toInfo;
    s32 found;
    s32 i;
    s32 canSwim;
    s32 fromWater;
    s32 toWater;
    s32 fromSpecial;
    s32 toSpecial;
    s32 climbs;
    s32 canClimb;
    f32 d;
    f32 fromY;
    f32 toY;
    f32 c0;
    f32 c1;
    f32 c2;
    f32 top;
    f32 top2;
    f32 y;

    to = from->m_pNeighbours[edge];
    if (to == NULL) {
        return NULL;
    }
    obj = sweep->obj;
    def = sweep->def;
    if (from->m_wFlags & 0x20) {
        found = 0;
        for (i = 0; i < 3; i++) {
            if (to->m_pNeighbours[i] == from) {
                found = 1;
                break;
            }
        }
        if (!found) {
            canSwim = 0;
            if ((def->m_dwFlags & 0x20000) || func_00267F10(obj)) {
                canSwim = 1;
            }
            if (!canSwim) {
                return NULL;
            }
            if (!func_0026BB9C(to, pos->x, pos->z)) {
                p.x = pos->x;
                p.y = obj->header.m_vPos.y;
                p.z = pos->z;
                to = func_0026B4A4(from, p);
                if (to == NULL) {
                    return NULL;
                }
            }
        }
    }
    fromInfo = func_0022425C(D_800F7078, from);
    toInfo = func_0022425C(D_800F7078, to);
    if (!(def->m_dwFlags & 0x10) && (REGION_WORD(to) & 6) == 2) {
        return NULL;
    }
    if (!(to->m_wFlags & 0x1000) && (from->m_wFlags & 0x1000) && !(def->m_dwFlags & 0x200) && !func_002680D0(obj)) {
        return NULL;
    }
    if ((to->m_wFlags & 0x1000) && !(from->m_wFlags & 0x1000) && !(def->m_dwFlags & 0x100) && !func_002680A8(obj)) {
        return NULL;
    }
    if (!(toInfo->m_wFlags54 & 2) && (fromInfo->m_wFlags54 & 2) && !(def->m_dwFlags & 0x800)) {
        return NULL;
    }
    if ((toInfo->m_wFlags54 & 2) && !(fromInfo->m_wFlags54 & 2) && !(def->m_dwFlags & 0x400)) {
        return NULL;
    }
    if ((to->m_wFlags & 0x40) && !(from->m_wFlags & 0x40) && func_00269C3C(to, pos->x, pos->z) < pos->y) {
        return NULL;
    }
    /* The do/while (0) block (a macro in the original?) keeps reorg's
     * delay-slot choices of the ROM. */
    do {
        if (obj->header.m_Type == 1 && (obj->m_AI.m_dwFlags & 0xC0000)) {
            return to;
        }
        if (!(toInfo->m_dwFlags & 0x400000) && (fromInfo->m_dwFlags & 0x400000) && !(def->m_dwFlags & 0x4000000)
            && !func_00268080(obj)) {
            return NULL;
        }
    } while (0);
    if ((toInfo->m_dwFlags & 0x400000) && !(fromInfo->m_dwFlags & 0x400000) && !(def->m_dwFlags & 0x2000000)
        && !func_0026803C(obj)) {
        return NULL;
    }
    if (!(toInfo->m_wFlags52 & 0x80) && (fromInfo->m_wFlags52 & 0x80) && !(def->m_dwFlags & 0x10000000)
        && !func_00268014(obj)) {
        return NULL;
    }
    if ((toInfo->m_wFlags52 & 0x80) && !(fromInfo->m_wFlags52 & 0x80) && !(def->m_dwFlags & 0x8000000)
        && !func_00267FD0(obj)) {
        return NULL;
    }
    if (!(def->m_dwFlags & 0x80)) {
        if (!(to->m_wFlags & 0x80) && (from->m_wFlags & 0x80) && !(def->m_dwFlags & 0x2000) && !func_00268120(obj)) {
            return NULL;
        }
        if ((to->m_wFlags & 0x80) && !(from->m_wFlags & 0x80) && !(def->m_dwFlags & 0x1000) && !func_002680F8(obj)) {
            return NULL;
        }
    }
    fromWater = 0;
    if (from->m_wFlags & 1) {
        fromWater = !isShallow(fromInfo);
    }
    toWater = 0;
    if (to->m_wFlags & 1) {
        toWater = !isShallow(toInfo);
    }
    if (!toWater && fromWater && !(def->m_dwFlags & 0x80000) && !func_00267FA8(obj)) {
        return NULL;
    }
    if (toWater && !fromWater && !(def->m_dwFlags & 0x40000) && !func_00267F64(obj)) {
        return NULL;
    }
    if ((to->m_wFlags & 0x40) && !(from->m_wFlags & 0x40) && func_00269C3C(to, pos->x, pos->z) < pos->y) {
        return NULL;
    }
    fromSpecial = func_0026AFC0(from);
    toSpecial = func_0026AFC0(to);
    if (sweep->def->m_dwFlags & 0x40) {
        if (toSpecial) {
            return to;
        }
        return NULL;
    }
    climbs = func_00267EA8(obj);
    canClimb = 0;
    if ((def->m_dwFlags & 0x10000) || func_00267EE8(obj)) {
        canClimb = 1;
    }
    canSwim = 0;
    if ((def->m_dwFlags & 0x20000) || func_00267F10(obj)) {
        canSwim = 1;
    }
    if (climbs && fromSpecial) {
        if (!toSpecial) {
            return to;
        }
        if ((from->m_wFlags & 0x10) && !(to->m_wFlags & 0x10)
            && obj->header.m_vPos.y < func_0026BE60(from, obj->header.m_vPos.x, obj->header.m_vPos.z) + 1.024f) {
            return NULL;
        }
    }
    if (!toSpecial || fromSpecial) {
        return to;
    }
    if ((to->m_wFlags & 0x10) && climbs) {
        angle = func_0026C240(from, edge) + 3.1415927f;
        func_00210EF0(&angle);
        d = func_00210E64(angle, obj->m_RotY);
        if ((d < 0.0f) ? (-d <= 0.6981318f) : (d <= 0.6981318f)) {
            canClimb = 1;
        }
    }
    if (canClimb && canSwim) {
        return to;
    }
    fromY = from->m_pVertices[0]->y + from->m_pVertices[1]->y + from->m_pVertices[2]->y;
    c0 = to->m_pVertices[0]->y;
    c1 = to->m_pVertices[1]->y;
    c2 = to->m_pVertices[2]->y;
    toY = c0 + c1 + c2;
    if (fromY < toY) {
        if (canClimb) {
            return to;
        }
        y = obj->header.m_vPos.y - 0.1f;
        top = c1;
        if (!(c0 <= top)) {
            top = c0;
        }
        top2 = c2;
        if (!(top <= top2)) {
            top2 = top;
        }
        if (top2 < y) {
            return to;
        }
        return NULL;
    }
    if (canSwim) {
        return to;
    }
    return NULL;
}
