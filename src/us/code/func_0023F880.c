#include "common.h"
#include "structs/game_object.h"

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 kind;
    /* 0x05 */ s8 hitKind;
    /* 0x06 */ u8 pad6[0xA];
    /* 0x10 */ f32 hitArg;
} SweepDef;

typedef struct HitDef {
    /* 0x00 */ s32 kind;
} HitDef;

/* An instance that can be hit, or the one sweeping (a particle: type 2). */
typedef struct HitInst {
    /* 0x000 */ u8 type;
    /* 0x002 */ u16 kind;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ HitDef *def;
    /* 0x018 */ f32 colour[3];
    /* 0x024 */ u8 pad24[0xE8];
    /* 0x10C */ struct HitInst *owner;
    /* 0x110 */ u8 pad110[4];
    /* 0x114 */ struct HitInst *target;
    /* 0x118 */ u8 pad118[0x90];
    /* 0x1A8 */ struct { u8 pad[0x4F4]; s32 hitByLaser; } *player;
} HitInst;

typedef struct HitTarget {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ CVector3 a;
    /* 0x14 */ CVector3 b;
} HitTarget;

/* What a hit on a model's box copies: 0x70 bytes (a matrix and more). */
typedef struct HitFrame {
    /* 0x00 */ s32 words[0x1C];
} HitFrame;

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
} Sweep;

typedef struct EffectParams {
    /* 0x0 */ s16 type;
    /* 0x2 */ s16 scale;
    /* 0x4 */ u8 count;
    /* 0x5 */ s8 r;
    /* 0x6 */ s8 g;
    /* 0x7 */ s8 b;
} __attribute__((aligned(4))) EffectParams;

extern HitTarget *D_800F1ADC;

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_0020EF8C(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 func_0023AB18(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 func_002682CC(HitInst *inst);
void func_00236EA0(HitInst *a, HitInst *b, s32 type, s32 arg3, CVector3 pos, EffectParams params);
void func_00275544(s32 sound, f32 x, f32 y, f32 z, s32 arg4, s32 arg5);

#define DOT(a, b) ((a).x * (b).x + (a).y * (b).y + (a).z * (b).z)

/* Sweep against the six faces of a box given by its eight corners. */
s32 func_0023F880(Sweep *sweep, s32 unused, HitInst *other, CVector3 *start, CVector3 *corners,
                  HitFrame *frame, s32 hitKind) {
    CVector3 *quad[4];
    CVector3 normal;
    CVector3 rel;
    CVector3 point;
    CVector3 edge;
    CVector3 toPoint;
    CVector3 pos;
    EffectParams params;
    s32 result;
    f32 denom;
    f32 num;
    f32 t;
    s32 face;
    s32 inside;
    s32 j;
    s32 next;
    CVector3 *cur;
    HitInst *inst;
    HitTarget *target;

    result = 0;
    for (face = 0; face < 6; face++) {
        switch (face) {
            case 0:
                quad[0] = &corners[7];
                quad[1] = &corners[6];
                quad[2] = &corners[5];
                quad[3] = &corners[4];
                func_0020EF2C(&normal, &corners[4], &corners[0]);
                break;
            case 1:
                quad[0] = &corners[4];
                quad[1] = &corners[5];
                quad[2] = &corners[1];
                quad[3] = &corners[0];
                func_0020EF2C(&normal, &corners[0], &corners[3]);
                break;
            case 2:
                quad[0] = &corners[7];
                quad[1] = &corners[4];
                quad[2] = &corners[0];
                quad[3] = &corners[3];
                func_0020EF2C(&normal, &corners[3], &corners[2]);
                break;
            case 3:
                quad[0] = &corners[6];
                quad[1] = &corners[7];
                quad[2] = &corners[3];
                quad[3] = &corners[2];
                func_0020EF2C(&normal, &corners[2], &corners[1]);
                break;
            case 4:
                quad[0] = &corners[5];
                quad[1] = &corners[6];
                quad[2] = &corners[2];
                quad[3] = &corners[1];
                func_0020EF2C(&normal, &corners[1], &corners[0]);
                break;
            case 5:
                quad[0] = &corners[0];
                quad[1] = &corners[1];
                quad[2] = &corners[2];
                quad[3] = &corners[3];
                func_0020EF2C(&normal, &corners[3], &corners[7]);
                break;
        }
        denom = DOT(normal, sweep->dir);
        if (denom == 0.0f) {
            continue;
        }
        func_0020EF2C(&rel, start, quad[0]);
        num = DOT(rel, normal);
        if (num == 0.0f) {
            continue;
        }
        t = -num / denom;
        if (!(t < sweep->maxT)) {
            continue;
        }
        point.x = start->x + t * sweep->dir.x;
        point.y = start->y + t * sweep->dir.y;
        point.z = start->z + t * sweep->dir.z;
        inside = 1;
        for (j = 0; j < 4; j++) {
            next = 0;
            if (j != 3) {
                next = j + 1;
            }
            cur = quad[j];
            func_0020EF2C(&edge, quad[next], cur);
            func_0020EF2C(&toPoint, &point, cur);
            if (DOT(edge, toPoint) < 0.0f) {
                inside = 0;
                break;
            }
        }
        if (!inside) {
            continue;
        }
        if (!func_0023AB18(sweep, &normal, quad[0], 0,
                           func_002682CC(other) ? sweep->def->hitKind : sweep->def->kind,
                           sweep->def->hitArg, 0)) {
            continue;
        }
        target = D_800F1ADC;
        sweep->hitKind = hitKind;
        sweep->hitA = &target->a;
        sweep->hitIndex = &target->unk4;
        sweep->hitB = &target->b;
        sweep->hitOwner = target;
        sweep->hitData = other;
        if (frame != NULL) {
            *(HitFrame *) ((u8 *) target + 0x18) = *frame;
        }
        inst = sweep->inst;
        result = 1;
        if (inst->type != 2) {
            continue;
        }
        if (inst->kind != 0x68) {
            continue;
        }
        if (inst->target == other) {
            continue;
        }
        inst->target = other;
        func_0020EF8C(&pos, sweep->maxT, &sweep->inst->pos, &sweep->end);
        params.type = 5;
        params.scale = 0x6400;
        params.count = 5;
        params.r = sweep->inst->colour[0];
        params.g = sweep->inst->colour[1];
        params.b = sweep->inst->colour[2];
        func_00236EA0(other, sweep->inst, 2, -1, pos, params);
        func_00275544(0x1B4, pos.x, pos.y, pos.z, 0, -1);
        if (other->def->kind == result) {
            sweep->inst->owner->player->hitByLaser = result;
        }
    }
    return result;
}
