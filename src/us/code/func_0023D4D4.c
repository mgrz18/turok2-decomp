#include "common.h"
#include "structs/game_object.h"

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ s8 hitKind;
    /* 0x06 */ u8 pad6[0xA];
    /* 0x10 */ f32 hitArg;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ f32 specialArg;
} SweepDef;

/* A segment swept from an instance's position to end, and what it hit. */
typedef struct Sweep {
    /* 0x00 */ CInstanceHeader *inst;
    /* 0x04 */ SweepDef *def;
    /* 0x08 */ f32 maxT;
    /* 0x0C */ u8 padC[0x28];
    /* 0x34 */ void *hitA;
    /* 0x38 */ s32 hitIndex;
    /* 0x3C */ void *hitB;
    /* 0x40 */ s32 hitKind;
    /* 0x44 */ void *hitOwner;
    /* 0x48 */ void *hitData;
    /* 0x4C */ u8 pad4C[0x50];
    /* 0x9C */ s32 unk9C;
} Sweep;

typedef struct HitTarget {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ CVector3 a;
    /* 0x14 */ CVector3 b;
    /* 0x20 */ u8 pad20[0xA4];
    /* 0xC4 */ s32 special;
    /* 0xC8 */ CVector3 specialA;
    /* 0xD4 */ CVector3 specialB;
} HitTarget;

/* A triangle corner projected to the sweep's space, with its model vertex. */
typedef struct ProjVertex {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s16 *pos;
} ProjVertex;

typedef struct ModelSweep {
    /* 0x00 */ Sweep *sweep;
    /* 0x04 */ u8 pad4[0x40];
    /* 0x44 */ f32 scale;
    /* 0x48 */ f32 hitDist;
    /* 0x4C */ struct { u8 pad[0x10]; void *data; } *model;
    /* 0x50 */ s32 kind;
    /* 0x54 */ f32 mtx[4][4];
} ModelSweep;

extern HitTarget *D_800F1ADC;

void func_0020F85C(f32 (*mtx)[4], CVector3 *in, CVector3 *out);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);
s32 func_0023AB18(Sweep *sweep, CVector3 *normal, CVector3 *point, s32 arg3, s32 arg4, f32 arg5, s32 arg6);

/* Sweep against one triangle of a model, given by its corners projected so
 * that the sweep's line is the origin: if the line passes through the front
 * of the triangle, the triangle is put in world space and the hit recorded. */
void func_0023D4D4(ModelSweep *ms, ProjVertex **v) {
    CVector3 pos;
    CVector3 world[3];
    CVector3 edge1;
    CVector3 edge2;
    CVector3 normal;
    ProjVertex *p;
    ProjVertex *q;
    Sweep *sweep;
    SweepDef *def;
    s16 *src;
    s32 i;
    f32 dx1;
    f32 dy1;
    f32 dx2;
    f32 dy2;
    s32 j;

    dx1 = v[1]->x - v[0]->x;
    dy1 = v[1]->y - v[0]->y;
    dx2 = v[2]->x - v[1]->x;
    dy2 = v[2]->y - v[1]->y;
    if (dx1 * dy2 - dy1 * dx2 >= 0.0f) {
        return;
    }
    for (i = 0; i < 3; i++) {
        p = v[i];
        q = v[i == 2 ? 0 : i + 1];
        if ((p->y - q->y) * p->x + (q->x - p->x) * p->y < 0.0f) {
            return;
        }
    }
    for (j = 0; j < 3; j++) {
        src = v[j]->pos;
        pos.x = src[0];
        pos.y = src[1];
        pos.z = src[2];
        func_0020F85C(ms->mtx, &pos, &world[j]);
    }
    func_0020EF2C(&edge1, &world[1], &world[0]);
    func_0020EF2C(&edge2, &world[2], &world[1]);
    func_0020EFDC(&normal, &edge2, &edge1);
    sweep = ms->sweep;
    def = sweep->def;
    switch (ms->kind) {
        case 1:
        case 7:
        case 8:
            if ((def->flags & 8) && D_800F1ADC->special == 0 &&
                func_0023AB18(sweep, &normal, &world[0], 1, 7, def->specialArg, 0)) {
                sweep->hitA = &D_800F1ADC->specialA;
                sweep->hitIndex = 0;
                sweep->hitB = &D_800F1ADC->specialB;
                sweep->hitKind = ms->kind;
                sweep->hitOwner = &D_800F1ADC->special;
                sweep->hitData = ms->model->data;
            }
            break;
        default:
            if (func_0023AB18(sweep, &normal, &world[0], 1, def->hitKind, def->hitArg, 0)) {
                ms->hitDist = ms->scale * sweep->maxT;
                sweep->hitA = &D_800F1ADC->a;
                sweep->hitB = &D_800F1ADC->b;
                sweep->hitIndex = 0;
                sweep->hitKind = -1;
                sweep->hitOwner = D_800F1ADC;
                sweep->hitData = ms->model;
                if (def->flags & 0x20000000) {
                    sweep->unk9C = 1;
                }
            }
            break;
    }
}
