#include "common.h"
#include "structs/game_object.h"

/* A navigation region: a triangle with its three corners and the regions
 * across each edge. */
typedef struct Region {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;               /* 0x8000: visited; 0x20: edges need a common neighbour */
    /* 0x04 */ CVector3 *corner[3];
    /* 0x10 */ struct Region *neighbour[3];
} Region;

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 kind;
    /* 0x05 */ s8 hitKind;
} SweepDef;

typedef struct SweepInst {
    /* 0x00 */ u8 type;
    /* 0x04 */ CVector3 pos;
} SweepInst;

/* A segment swept from an instance's position to end, and what it hit. */
typedef struct Sweep {
    /* 0x00 */ SweepInst *inst;
    /* 0x04 */ SweepDef *def;
    /* 0x08 */ f32 maxT;
    /* 0x0C */ u8 padC[0x28];
    /* 0x34 */ void *hitA;
    /* 0x38 */ void *hitIndex;
    /* 0x3C */ void *hitB;
    /* 0x40 */ s32 hitKind;
    /* 0x44 */ void *hitOwner;
    /* 0x48 */ void *hitData;
    /* 0x4C */ CVector3 end;
    /* 0x58 */ CVector3 dir;
    /* 0x64 */ CVector3 missCentre;     /* last cylinder found not hit */
    /* 0x70 */ CVector3 hitCentre;
    /* 0x7C */ f32 missRadius;
    /* 0x80 */ f32 hitRadius;
    /* 0x84 */ s32 missKind;
} Sweep;

/* The state of a sweep against the map's walls (set up by func_00240E90). */
typedef struct MapSweep {
    /* 0x00 */ Sweep *sweep;
    /* 0x04 */ CVector3 *a;             /* the edge's corners */
    /* 0x08 */ CVector3 *b;
    /* 0x0C */ CVector3 side;           /* the sweep's direction turned a right angle */
    /* 0x18 */ CVector3 edge;           /* b - a */
    /* 0x24 */ CVector3 toEnd;
    /* 0x30 */ CVector3 wallPoint;      /* a pushed out by the radius */
    /* 0x3C */ CVector3 normal;         /* the edge's normal */
    /* 0x48 */ CVector3 toStart;
    /* 0x54 */ f32 radius;
    /* 0x58 */ f32 len;
    /* 0x5C */ f32 startSide;
    /* 0x60 */ f32 across;
    /* 0x64 */ f32 endSide;
    /* 0x68 */ f32 t;
    /* 0x6C */ Region *next;
    /* 0x70 */ s32 hit;
    /* 0x74 */ f32 minX;
    /* 0x78 */ f32 minZ;
    /* 0x7C */ f32 maxX;
    /* 0x80 */ f32 maxZ;
} MapSweep;

typedef struct HitTarget {
    /* 0x00 */ u8 pad0[0x9C];
    /* 0x9C */ s32 wallIndex;
    /* 0xA0 */ CVector3 wall;
    /* 0xAC */ s32 edge;
} HitTarget;

extern MapSweep *D_800F1AB0;
extern HitTarget *D_800F1ADC;
extern u8 D_800C894C;                   /* bottom of the stack */

f32 func_00298470(f32 x);               /* sqrtf */
s32 func_0023AB18(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 func_0023B38C(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside);
Region *func_0023B664(Sweep *mover, Region *region, s32 edge, CVector3 *pos);

/* Walk the regions the sweep's box overlaps, from region, sweeping the
 * sweep's circle against each wall edge (and the corners at its ends). */
void func_00240800(Region *region) {
    u8 cornerDone[3];
    s32 i;

    if ((u32) &region < (u32) &D_800C894C) {
        return;
    }
    i = 0;
    region->flags |= 0x8000;
    cornerDone[2] = 0;
    cornerDone[1] = 0;
    cornerDone[0] = 0;
    do {
        D_800F1AB0->next = region->neighbour[i];
        if (D_800F1AB0->next != NULL) {
            if (D_800F1AB0->next->flags & 0x8000) {
                continue;
            }
            if (region->flags & 0x20) {
                if (D_800F1AB0->next->neighbour[0] != region && D_800F1AB0->next->neighbour[1] != region &&
                    D_800F1AB0->next->neighbour[2] != region) {
                    continue;
                }
            } else {
                D_800F1AB0->next = func_0023B664(D_800F1AB0->sweep, region, i, &D_800F1AB0->sweep->inst->pos);
            }
        }
        D_800F1AB0->a = region->corner[i];
        D_800F1AB0->b = region->corner[i == 2 ? 0 : i + 1];
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
        D_800F1AB0->toStart.x = D_800F1AB0->sweep->inst->pos.x - D_800F1AB0->a->x;
        D_800F1AB0->toStart.z = D_800F1AB0->sweep->inst->pos.z - D_800F1AB0->a->z;
        D_800F1AB0->startSide = D_800F1AB0->normal.x * D_800F1AB0->toStart.x +
                                D_800F1AB0->normal.z * D_800F1AB0->toStart.z;
        if (!(D_800F1AB0->startSide >= 0.0f)) {
            continue;
        }
        D_800F1AB0->side.x = -D_800F1AB0->sweep->dir.z;
        D_800F1AB0->side.z = D_800F1AB0->sweep->dir.x;
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
                                             D_800F1AB0->sweep->def->hitKind, 0.0f, 1);
        }
        if (!cornerDone[i == 2 ? 0 : i + 1]) {
            cornerDone[i == 2 ? 0 : i + 1] = 1;
            D_800F1AB0->hit |= func_0023B38C(D_800F1AB0->sweep, D_800F1AB0->b, D_800F1AB0->radius, 0, 0.0f, 0.0f, 1,
                                             D_800F1AB0->sweep->def->hitKind, 0.0f, 1);
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
                !(D_800F1AB0->sweep->missKind == 2 && D_800F1AB0->sweep->missRadius == D_800F1AB0->radius &&
                  D_800F1AB0->sweep->missCentre.x == D_800F1AB0->a->x &&
                  D_800F1AB0->sweep->missCentre.y == D_800F1AB0->a->y &&
                  D_800F1AB0->sweep->missCentre.z == D_800F1AB0->a->z) &&
                !(D_800F1AB0->sweep->missKind == 2 && D_800F1AB0->sweep->missRadius == D_800F1AB0->radius &&
                  D_800F1AB0->sweep->missCentre.x == D_800F1AB0->b->x &&
                  D_800F1AB0->sweep->missCentre.y == D_800F1AB0->b->y &&
                  D_800F1AB0->sweep->missCentre.z == D_800F1AB0->b->z)) {
                D_800F1AB0->hit |= func_0023AB18(D_800F1AB0->sweep, &D_800F1AB0->normal, &D_800F1AB0->wallPoint, 0,
                                                 D_800F1AB0->sweep->def->hitKind, 0.0f, 1);
            }
        }
        if (D_800F1AB0->hit == 0) {
            continue;
        }
        D_800F1AB0->sweep->hitA = &D_800F1ADC->wall;
        D_800F1AB0->sweep->hitIndex = &D_800F1ADC->wallIndex;
        D_800F1AB0->sweep->hitB = NULL;
        D_800F1AB0->sweep->hitKind = 0;
        D_800F1AB0->sweep->hitOwner = NULL;
        D_800F1AB0->sweep->hitData = NULL;
        D_800F1ADC->edge = i;
        if (D_800F1AB0->sweep->def->flags & 0x20000000) {
            return;
        }
    } while (++i < 3);
}
