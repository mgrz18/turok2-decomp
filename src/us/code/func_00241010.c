#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

/* A region corner: its floor point and the ceiling height above it. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 ceiling;
} Corner;

/* A triangle of the map's walkable graph. */
typedef struct Region {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u16 flags;           /* 1: water, 8: blocks some, 0x40: ceiling */
    /* 0x04 */ Corner *corners[3];
} Region;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 level;
    /* 0x24 */ u8 pad24[0x44 - 0x24];
    /* 0x44 */ u32 flags;
    /* 0x48 */ u8 pad48[0x52 - 0x48];
    /* 0x52 */ u16 flags52;
} Water;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Region *region;
} SweepInst;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ s8 hitKind;
    /* 0x06 */ s8 hitsFloor;
    /* 0x07 */ u8 pad7[0x10 - 7];
    /* 0x10 */ f32 hitArg;
} SweepDef;

/* A move from an instance's position along `move`, and what it hit. */
typedef struct {
    /* 0x00 */ SweepInst *inst;
    /* 0x04 */ SweepDef *def;
    /* 0x08 */ f32 maxT;
    /* 0x0C */ u8 padC[0x34 - 0xC];
    /* 0x34 */ void *hitA;
    /* 0x38 */ void *hitIndex;
    /* 0x3C */ void *hitB;
    /* 0x40 */ s32 hitKind;
    /* 0x44 */ s32 hitOwner;
    /* 0x48 */ s32 hitData;
    /* 0x4C */ Vec3f move;
    /* 0x58 */ Vec3f dir;
    /* 0x64 */ Vec3f lastNormal;
    /* 0x70 */ u8 pad70[0x84 - 0x70];
    /* 0x84 */ s32 lastKind;
    /* 0x88 */ u8 pad88[0x94 - 0x88];
    /* 0x94 */ s32 unk94;
    /* 0x98 */ s32 unk98;
} Sweep;

/* The collision result block (D_800F1CE0). */
typedef struct {
    /* 0x00 */ u8 pad0[0x9C];
    /* 0x9C */ s32 edgeIndex;
    /* 0xA0 */ s32 edgeA;
    /* 0xA4 */ u8 padA4[0xAC - 0xA4];
    /* 0xAC */ s32 edge;
    /* 0xB0 */ s32 floorB;
    /* 0xB4 */ s32 floorIndex;
    /* 0xB8 */ s32 floorA;
    /* 0xBC */ u8 padBC[0xC4 - 0xBC];
    /* 0xC4 */ s32 waterIndex;
    /* 0xC8 */ Vec3f waterA;
    /* 0xD4 */ Vec3f waterB;
} HitTarget;

extern HitTarget *D_800F1ADC;
extern u8 D_800F7078[];

void func_00266C5C(void *p, s32 c, s32 n);                 /* memset */
s32 func_0023C188(Sweep *sweep, Region *region);
void func_0026BC54(Vec3f *normal, Region *region);        /* floor plane */
void func_0026BFB4(Vec3f *normal, Region *region);        /* ceiling plane */
s32 func_0026AFC0(Region *region);
Water *func_0022425C(u8 *scene, Region *region);
f32 func_002671B4(SweepInst *inst);                       /* height */
void func_0020EF2C(Vec3f *out, void *a, Vec3f *b);        /* out = a - b */
void func_0020EF8C(Vec3f *out, f32 t, Vec3f *base, Vec3f *dir);   /* out = base + t * dir */
Region *func_0023B664(Sweep *sweep, Region *region, s32 edge, Vec3f *point);
s32 func_0023AB18(Sweep *sweep, Vec3f *normal, void *point, s32 arg3, s32 arg4, f32 arg5, s32 arg6);

static __inline__ f32 Dot(Vec3f *a, Vec3f *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* Walk a sweep through the map's regions: in each, find where it leaves
 * through the floor, the ceiling, the water surface or an edge, record the
 * nearest hit, or cross the edge into the next region (100 at most). */
void func_00241010(Sweep *sweep) {
    Vec3f edgeN;
    Vec3f floorN;
    Vec3f ceilN;
    Vec3f up = { 0.0f, 1.0f, 0.0f };
    Vec3f down = { 0.0f, -1.0f, 0.0f };
    Vec3f surface;
    Vec3f rel;
    Vec3f ceilPt;
    Vec3f at;
    SweepDef *def;
    Vec3f *pos;
    Region **regionp;
    Region *next;
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

    func_00266C5C(&surface, 0, sizeof(surface));
    if ((sweep->def->flags & 0x1000000) || sweep->inst->region == NULL || sweep->def->hitKind == 0) {
        return;
    }
    def = sweep->def;
    pos = &sweep->inst->pos;
    regionp = &sweep->inst->region;
    count = 0;
    best = -10000.0f;
    do {
        floorT = 1.0f;
        if (sweep->unk98 != 0) {
            v = func_0023C188(sweep, sweep->inst->region);
        } else {
            v = sweep->def->hitsFloor;
        }
        if (sweep->unk94 == 0 && v != 0) {
            func_0026BC54(&floorN, *regionp);
            d = floorN.x * sweep->dir.x + floorN.y * sweep->dir.y + floorN.z * sweep->dir.z;
            if (d < 0.0f) {
                func_0020EF2C(&rel, (*regionp)->corners[0], pos);
                num = Dot(&floorN, &rel);
                floorT = num / d;
                if (floorT < best) {
                    floorT = 1.0f;
                }
            }
        }
        ceilT = 1.0f;
        if (sweep->def->hitsFloor != 0 && ((*regionp)->flags & 0x40)) {
            func_0026BFB4(&ceilN, *regionp);
            d = ceilN.x * sweep->dir.x + ceilN.y * sweep->dir.y + ceilN.z * sweep->dir.z;
            if (d < 0.0f) {
                ceilPt.x = (*regionp)->corners[0]->x;
                ceilPt.y = (*regionp)->corners[0]->ceiling;
                if (sweep->def->flags & 0x400000) {
                    ceilPt.y -= func_002671B4(sweep->inst);
                    if (sweep->def->flags & 0x80) {
                        ceilPt.y += 44.032f;
                    }
                }
                ceilPt.z = (*regionp)->corners[0]->z;
                func_0020EF2C(&rel, &ceilPt, pos);
                num = Dot(&ceilN, &rel);
                ceilT = num / d;
                if (ceilT < best) {
                    ceilT = 1.0f;
                }
            }
        }
        waterT = 1.0f;
        water = func_0022425C(D_800F7078, *regionp);
        if (((*regionp)->flags & 1) && (sweep->def->flags & 8) && D_800F1ADC->waterIndex == 0 && water != NULL
            && sweep->dir.y != 0.0f) {
            waterT = (water->level - pos->y) / sweep->dir.y;
            if (waterT < best) {
                waterT = 1.0f;
            }
        }
        best = sweep->maxT;
        edge = 0;
        i = 0;
        do {
            a = (*regionp)->corners[i];
            b = (*regionp)->corners[(i != 2) ? i + 1 : 0];
            ex = b->z - a->z;
            ez = a->x - b->x;
            d = ex * sweep->dir.x + ez * sweep->dir.z;
            if (d < 0.0f) {
                t = (ex * (a->x - pos->x) + ez * (a->z - pos->z)) / d;
                if (t < best
                    && (sweep->lastKind != 1 || sweep->lastNormal.x != ex || sweep->lastNormal.y != 0.0f
                        || sweep->lastNormal.z != ez)) {
                    best = t;
                    edge = i;
                    edgeN.x = ex;
                    edgeN.z = ez;
                }
            }
            i++;
        } while (i < 3);
        if (waterT <= best && waterT < sweep->maxT) {
            surface.y = water->level;
            if (func_0023AB18(sweep, (pos->y >= water->level) ? &up : &down, &surface, 1, 7, 0.0f, 1)) {
                if (water->flags & 0x400000) {
                    kind = 7;
                } else if (water->flags52 & 0x80) {
                    kind = 8;
                } else {
                    kind = 1;
                }
                sweep->hitKind = kind;
                sweep->hitOwner = 0;
                sweep->hitData = 0;
                sweep->hitA = &D_800F1ADC->waterA;
                sweep->hitIndex = &D_800F1ADC->waterIndex;
                sweep->hitB = &D_800F1ADC->waterB;
            }
        }
        if (floorT <= best && floorT < sweep->maxT) {
            if (func_0026AFC0(*regionp) == 0) {
                arg = sweep->def->hitArg;
            } else {
                arg = 0.0f;
            }
            if (func_0023AB18(sweep, &floorN, (*regionp)->corners[0], 0, sweep->def->hitsFloor, arg, 1)) {
                sweep->hitKind = 1;
                sweep->hitOwner = 0;
                sweep->hitData = 0;
                sweep->hitA = &D_800F1ADC->floorA;
                sweep->hitIndex = &D_800F1ADC->floorIndex;
                sweep->hitB = &D_800F1ADC->floorB;
                if (def->flags & 0x20000000) {
                    return;
                }
            }
        }
        if (ceilT <= best && ceilT < sweep->maxT) {
            if (func_0023AB18(sweep, &ceilN, &ceilPt, 0, sweep->def->hitKind, 0.0f, 1)) {
                sweep->hitKind = 2;
                sweep->hitOwner = 0;
                sweep->hitData = 0;
                sweep->hitA = &D_800F1ADC->floorA;
                sweep->hitIndex = &D_800F1ADC->floorIndex;
                sweep->hitB = &D_800F1ADC->floorB;
                if (def->flags & 0x20000000) {
                    return;
                }
            }
        }
        if (!(best < sweep->maxT)) {
            return;
        }
        func_0020EF8C(&at, best, pos, &sweep->move);
        next = func_0023B664(sweep, *regionp, edge, &at);
        if (next == NULL || sweep->unk94 != 0 || ((sweep->def->flags & 0x8000) && (next->flags & 8))) {
            edgeN.y = 0.0f;
            if (func_0023AB18(sweep, &edgeN, (*regionp)->corners[edge], 0, (next != NULL) ? 7 : sweep->def->hitKind, 0.0f, 1)) {
                if (next != NULL) {
                    *regionp = next;
                    sweep->hitA = NULL;
                    sweep->hitIndex = NULL;
                    sweep->hitB = NULL;
                    sweep->hitKind = 0;
                    sweep->hitOwner = 0;
                    sweep->hitData = 0;
                    return;
                }
                sweep->hitB = NULL;
                sweep->hitKind = 0;
                sweep->hitOwner = 0;
                sweep->hitData = 0;
                sweep->hitA = &D_800F1ADC->edgeA;
                sweep->hitIndex = &D_800F1ADC->edgeIndex;
                D_800F1ADC->edge = edge;
            }
            return;
        }
        count++;
        *regionp = next;
    } while (count < 100);
}
