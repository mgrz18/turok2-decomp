#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

/* A triangle corner projected to the sweep's space, with its model vertex. */
typedef struct ProjVertex {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s16 *pos;
} ProjVertex;

/* The scene model a ModelSweep is against: its data at 0x10. */
typedef struct SweptModel {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ void *data;
} SweptModel;

extern CCollisionResults *D_800F1ADC;

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
    CCollisionInfo *def;
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
            if ((def->m_dwFlags & 8) && D_800F1ADC->m_pWaterRegion == 0 &&
                func_0023AB18(sweep, &normal, &world[0], 1, 7, def->m_WaterFriction, 0)) {
                sweep->hit.posOut = &D_800F1ADC->m_vWaterPos;
                sweep->hit.regionOut = NULL;
                sweep->hit.kindOut = &D_800F1ADC->m_nWaterKind;
                sweep->hit.kind = ms->kind;
                sweep->hit.dataOut = (void **)&D_800F1ADC->m_pWaterRegion;
                sweep->hit.data = ((SweptModel *)ms->model)->data;
            }
            break;
        default:
            if (func_0023AB18(sweep, &normal, &world[0], 1, def->m_WallBehavior, def->m_GroundFriction, 0)) {
                ms->hitDist = ms->scale * sweep->hit.fraction;
                sweep->hit.posOut = &D_800F1ADC->m_vInstancePos;
                sweep->hit.kindOut = &D_800F1ADC->m_nInstanceNode;
                sweep->hit.regionOut = NULL;
                sweep->hit.kind = -1;
                sweep->hit.dataOut = (void **)&D_800F1ADC->m_pInstance;
                sweep->hit.data = ms->model;
                if (def->m_dwFlags & 0x20000000) {
                    sweep->stop = 1;
                }
            }
            break;
    }
}
