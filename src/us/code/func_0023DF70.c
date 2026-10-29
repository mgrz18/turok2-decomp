#include "common.h"
#include "structs/game_object.h"

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 kind;
} SweepDef;

typedef struct HitDef {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s32 flags;
} HitDef;

/* An instance that can be hit (or the instance sweeping). */
typedef struct HitInst {
    /* 0x00 */ u8 type;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ HitDef *def;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 fallSpeed;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ struct HitInst *standingOn;
} HitInst;

typedef struct HitTarget {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ CVector3 a;
    /* 0x14 */ CVector3 b;
} HitTarget;

/* A segment swept from an instance's position to end, and what it hit. */
typedef struct Sweep {
    /* 0x00 */ HitInst *inst;
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
    /* 0x70 */ CVector3 hitCentre;      /* cylinder hit */
    /* 0x7C */ f32 missRadius;
    /* 0x80 */ f32 hitRadius;
    /* 0x84 */ s32 missKind;
    /* 0x88 */ s32 hitShape;
} Sweep;

/* The volume the sweeping instance covers (built by func_0023E4A0). */
typedef struct SweepBox {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 bottom;
    /* 0x08 */ f32 top;
    /* 0x0C */ f32 startTop;
    /* 0x10 */ f32 startBottom;
    /* 0x14 */ f32 endTop;
    /* 0x18 */ f32 endBottom;
    /* 0x1C */ f32 minX;
    /* 0x20 */ f32 minZ;
    /* 0x24 */ f32 maxX;
    /* 0x28 */ f32 maxZ;
    /* 0x2C */ s32 flag;
} SweepBox;

typedef struct Rect {
    /* 0x0 */ f32 minX;
    /* 0x4 */ f32 minZ;
    /* 0x8 */ f32 maxX;
    /* 0xC */ f32 maxZ;
} Rect;

extern HitTarget *D_800F1ADC;

f32 func_002671B4(HitInst *inst);
f32 func_002672C8(HitInst *inst);
f32 func_0026841C(HitInst *inst);
void func_0020EF8C(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 func_0023AB18(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 func_0023B38C(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside);
s32 func_0023F034(HitInst *other, HitInst *inst);
void func_0023C0FC(f32 *hit);
void func_00241CB0(Sweep *sweep, HitInst *other);
void func_00242604(Sweep *sweep, HitInst *other);

/* Sweep an instance's volume (box) against another instance's cylinder:
 * its top and bottom discs when the box asks for it, then its side. */
void func_0023DF70(Sweep *sweep, SweepBox *box, HitInst *other) {
    Rect rect;
    CVector3 hit;
    CVector3 normal;
    f32 x;
    f32 z;
    f32 radius;
    f32 reach;
    f32 bottom;
    f32 top;
    f32 t;
    f32 dx;
    f32 dz;
    HitInst *inst;
    SweepDef *def;
    HitTarget *target;
    s32 result;
    s32 special;

    x = other->pos.x;
    z = other->pos.z;
    radius = func_002672C8(other);
    rect.minX = x - radius;
    rect.maxX = x + radius;
    rect.minZ = z - radius;
    rect.maxZ = z + radius;
    if (!(rect.minX < box->maxX) || !(box->minX < rect.maxX) || !(rect.minZ < box->maxZ) ||
        !(box->minZ < rect.maxZ)) {
        return;
    }
    inst = sweep->inst;
    def = sweep->def;
    special = 0;
    if (inst->type == 1 && inst->def->kind == 2) {
        if (other->type == inst->type) {
            func_00241CB0(sweep, other);
        }
        return;
    }
    if (other->type == 1 && other->def->kind == 2) {
        if (inst->type == other->type) {
            func_00242604(sweep, other);
        }
        return;
    }
    if (other->type == 3) {
        if (!func_0023F034(other, inst)) {
            return;
        }
        special = 1;
    }
    bottom = other->pos.y + func_0026841C(other);
    top = func_002671B4(other) + bottom;
    if (box->flag) {
        normal.x = normal.z = 0.0f;
    }
    result = 0;
    reach = box->radius + radius;
    if (box->flag && sweep->dir.y != 0.0f) {
        if (sweep->missKind == 2 && sweep->missRadius == reach && sweep->missCentre.x == other->pos.x &&
            sweep->missCentre.y == other->pos.y && sweep->missCentre.z == other->pos.z) {
            return;
        }
        if (box->startTop <= bottom && bottom < box->endTop) {
            t = (bottom - box->startTop) / sweep->dir.y;
            if (t < sweep->maxT) {
                func_0020EF8C(&hit, t, &inst->pos, &sweep->end);
                dx = x - hit.x;
                dz = z - hit.z;
                if (dx * dx + dz * dz < reach * reach) {
                    normal.y = -1.0f;
                    result |= func_0023AB18(sweep, &normal, &hit, 0, 7, 0.0f, 1);
                }
            }
        }
        if (top <= box->startBottom && box->endBottom < top) {
            t = (top - box->startBottom) / sweep->dir.y;
            if (t < sweep->maxT) {
                func_0020EF8C(&hit, t, &inst->pos, &sweep->end);
                dx = x - hit.x;
                dz = z - hit.z;
                if (dx * dx + dz * dz < reach * reach) {
                    normal.y = 1.0f;
                    if (other->def->flags & 0x80) {
                        if (func_0023AB18(sweep, &normal, &hit, 0, 2, 0.0f, 1)) {
                            result |= 1;
                            if (inst->fallSpeed < 0.0f) {
                                inst->fallSpeed = 0.0f;
                                inst->standingOn = other;
                            }
                        }
                    } else {
                        result |= func_0023AB18(sweep, &normal, &hit, 0, 7, 0.0f, 1);
                    }
                }
            }
        }
    }
    result |= func_0023B38C(sweep, &other->pos, reach, 1, top - box->bottom, bottom - box->top, 1,
                            special ? 7 : def->kind, 0.0f, 1);
    if (result) {
        target = D_800F1ADC;
        if (target->active) {
            func_0023C0FC(&sweep->maxT);
        } else {
            sweep->hitA = &target->a;
            sweep->hitIndex = &target->unk4;
            sweep->hitB = &target->b;
            sweep->hitKind = -1;
            sweep->hitOwner = target;
            sweep->hitData = other;
        }
        if (box->flag) {
            sweep->hitShape = 2;
            sweep->hitCentre = other->pos;
            sweep->hitRadius = reach;
        }
    }
}
