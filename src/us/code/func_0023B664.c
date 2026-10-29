#include "common.h"
#include "structs/game_object.h"

/* A navigation region: a triangle with its three corners and the regions
 * across each edge (see func_002399F8). */
typedef struct Region {
    union {
        /* 0x00 */ s32 word;            /* (word & 6) == 2: closed */
        struct {
            /* 0x00 */ u16 unk0;
            /* 0x02 */ u16 flags;
        } h;
    } u;
    /* 0x04 */ CVector3 *corner[3];
    /* 0x10 */ struct Region *neighbour[3];
} Region;

typedef struct RegionInfo {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0xA];
    /* 0x52 */ u16 flags52;
    /* 0x54 */ u16 flags54;
} RegionInfo;

/* What moves: the start of any instance. */
typedef struct Mover {
    /* 0x000 */ u8 type;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x40];
    /* 0x050 */ f32 rotY;
    /* 0x054 */ u8 pad54[0xEC];
    /* 0x140 */ s32 aiFlags;
} Mover;

typedef struct CollisionDef {
    /* 0x00 */ s32 flags;
} CollisionDef;

typedef struct Sweep {
    /* 0x00 */ Mover *obj;
    /* 0x04 */ CollisionDef *def;
} Sweep;

extern s32 D_800F7078[];

RegionInfo *func_0022425C(s32 *list, Region *region);
s32 func_00267EA8(Mover *obj);
s32 func_00267EE8(Mover *obj);
s32 func_00267F10(Mover *obj);
s32 func_00267F64(Mover *obj);
s32 func_00267FA8(Mover *obj);
s32 func_00267FD0(Mover *obj);
s32 func_00268014(Mover *obj);
s32 func_0026803C(Mover *obj);
s32 func_00268080(Mover *obj);
s32 func_002680A8(Mover *obj);
s32 func_002680D0(Mover *obj);
s32 func_002680F8(Mover *obj);
s32 func_00268120(Mover *obj);
s32 func_0026AFC0(Region *region);
s32 func_0026BB9C(Region *region, f32 x, f32 z);
Region *func_0026B4A4(Region *region, CVector3 pos);
f32 func_00269C3C(Region *region, f32 x, f32 z);
f32 func_0026BE60(Region *region, f32 x, f32 z);
f32 func_0026C240(Region *region, s32 edge);
void func_00210EF0(f32 *angle);
f32 func_00210E64(f32 a, f32 b);

/* Whether the region's water is only a surface (not swum in). */
static __inline__ s32 isShallow(RegionInfo *info) {
    return info->flags & 0x20000000;
}

/* Whether the sweep's instance may go from region `from` across its edge to
 * the neighbouring region: returns that region (or the one found under pos
 * when the link is one-way), or NULL. */
Region *func_0023B664(Sweep *sweep, Region *from, s32 edge, CVector3 *pos) {
    CVector3 p;
    f32 angle;
    Region *to;
    Mover *obj;
    CollisionDef *def;
    RegionInfo *fromInfo;
    RegionInfo *toInfo;
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

    to = from->neighbour[edge];
    if (to == NULL) {
        return NULL;
    }
    obj = sweep->obj;
    def = sweep->def;
    if (from->u.h.flags & 0x20) {
        found = 0;
        for (i = 0; i < 3; i++) {
            if (to->neighbour[i] == from) {
                found = 1;
                break;
            }
        }
        if (!found) {
            canSwim = 0;
            if ((def->flags & 0x20000) || func_00267F10(obj)) {
                canSwim = 1;
            }
            if (!canSwim) {
                return NULL;
            }
            if (!func_0026BB9C(to, pos->x, pos->z)) {
                p.x = pos->x;
                p.y = obj->pos.y;
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
    if (!(def->flags & 0x10) && (to->u.word & 6) == 2) {
        return NULL;
    }
    if (!(to->u.h.flags & 0x1000) && (from->u.h.flags & 0x1000) && !(def->flags & 0x200) && !func_002680D0(obj)) {
        return NULL;
    }
    if ((to->u.h.flags & 0x1000) && !(from->u.h.flags & 0x1000) && !(def->flags & 0x100) && !func_002680A8(obj)) {
        return NULL;
    }
    if (!(toInfo->flags54 & 2) && (fromInfo->flags54 & 2) && !(def->flags & 0x800)) {
        return NULL;
    }
    if ((toInfo->flags54 & 2) && !(fromInfo->flags54 & 2) && !(def->flags & 0x400)) {
        return NULL;
    }
    if ((to->u.h.flags & 0x40) && !(from->u.h.flags & 0x40) && func_00269C3C(to, pos->x, pos->z) < pos->y) {
        return NULL;
    }
    /* The do/while (0) block (a macro in the original?) keeps reorg's
     * delay-slot choices of the ROM. */
    do {
        if (obj->type == 1 && (obj->aiFlags & 0xC0000)) {
            return to;
        }
        if (!(toInfo->flags & 0x400000) && (fromInfo->flags & 0x400000) && !(def->flags & 0x4000000)
            && !func_00268080(obj)) {
            return NULL;
        }
    } while (0);
    if ((toInfo->flags & 0x400000) && !(fromInfo->flags & 0x400000) && !(def->flags & 0x2000000)
        && !func_0026803C(obj)) {
        return NULL;
    }
    if (!(toInfo->flags52 & 0x80) && (fromInfo->flags52 & 0x80) && !(def->flags & 0x10000000)
        && !func_00268014(obj)) {
        return NULL;
    }
    if ((toInfo->flags52 & 0x80) && !(fromInfo->flags52 & 0x80) && !(def->flags & 0x8000000)
        && !func_00267FD0(obj)) {
        return NULL;
    }
    if (!(def->flags & 0x80)) {
        if (!(to->u.h.flags & 0x80) && (from->u.h.flags & 0x80) && !(def->flags & 0x2000) && !func_00268120(obj)) {
            return NULL;
        }
        if ((to->u.h.flags & 0x80) && !(from->u.h.flags & 0x80) && !(def->flags & 0x1000) && !func_002680F8(obj)) {
            return NULL;
        }
    }
    fromWater = 0;
    if (from->u.h.flags & 1) {
        fromWater = !isShallow(fromInfo);
    }
    toWater = 0;
    if (to->u.h.flags & 1) {
        toWater = !isShallow(toInfo);
    }
    if (!toWater && fromWater && !(def->flags & 0x80000) && !func_00267FA8(obj)) {
        return NULL;
    }
    if (toWater && !fromWater && !(def->flags & 0x40000) && !func_00267F64(obj)) {
        return NULL;
    }
    if ((to->u.h.flags & 0x40) && !(from->u.h.flags & 0x40) && func_00269C3C(to, pos->x, pos->z) < pos->y) {
        return NULL;
    }
    fromSpecial = func_0026AFC0(from);
    toSpecial = func_0026AFC0(to);
    if (sweep->def->flags & 0x40) {
        if (toSpecial) {
            return to;
        }
        return NULL;
    }
    climbs = func_00267EA8(obj);
    canClimb = 0;
    if ((def->flags & 0x10000) || func_00267EE8(obj)) {
        canClimb = 1;
    }
    canSwim = 0;
    if ((def->flags & 0x20000) || func_00267F10(obj)) {
        canSwim = 1;
    }
    if (climbs && fromSpecial) {
        if (!toSpecial) {
            return to;
        }
        if ((from->u.h.flags & 0x10) && !(to->u.h.flags & 0x10)
            && obj->pos.y < func_0026BE60(from, obj->pos.x, obj->pos.z) + 1.024f) {
            return NULL;
        }
    }
    if (!toSpecial || fromSpecial) {
        return to;
    }
    if ((to->u.h.flags & 0x10) && climbs) {
        angle = func_0026C240(from, edge) + 3.1415927f;
        func_00210EF0(&angle);
        d = func_00210E64(angle, obj->rotY);
        if ((d < 0.0f) ? (-d <= 0.6981318f) : (d <= 0.6981318f)) {
            canClimb = 1;
        }
    }
    if (canClimb && canSwim) {
        return to;
    }
    fromY = from->corner[0]->y + from->corner[1]->y + from->corner[2]->y;
    c0 = to->corner[0]->y;
    c1 = to->corner[1]->y;
    c2 = to->corner[2]->y;
    toY = c0 + c1 + c2;
    if (fromY < toY) {
        if (canClimb) {
            return to;
        }
        y = obj->pos.y - 0.1f;
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
