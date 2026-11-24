#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "structs/rect.h"
#include "globals/scene.h"
#include "protos/iset.h"
#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* A rectangle on the ground plane. */
/* A scene model's bounds (what Sweep_HitBox tests). */
typedef struct ModelBounds {
    /* 0x00 */ CBox box;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ u16 flags;               /* 0x10: no collisions */
    /* 0x22 */ u8 pad22[6];
} ModelBounds;
/* One model instance of the scene, as listed for collisions. */
#ifdef TARGET_PORT
/* the host CStaticObject (0xA0): its bounds by offsetof, the view the record's full size */
#include "structs/level_actor.h"
typedef struct SceneModel {
    u8 data[__builtin_offsetof(CStaticObject, m_Bounds)];
    ModelBounds bounds;
} SceneModel;
_Static_assert(sizeof(SceneModel) == sizeof(CStaticObject), "SceneModel size");
_Static_assert(__builtin_offsetof(CStaticObject, m_wFlags) - __builtin_offsetof(CStaticObject, m_Bounds) == 0x20, "bounds flags");
#else
typedef struct SceneModel {
    /* 0x00 */ u8 data[0x68];
    /* 0x68 */ ModelBounds bounds;
} SceneModel; /* size 0x90 */
#endif

typedef struct SceneModels {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 count;
    /* 0x08 */ SceneModel models[1];
} SceneModels;
/* A cell of the scene grid: its models and their bounds. */
typedef struct SceneCell {
    /* 0x00 */ s32 **data;
    /* 0x04 */ CRect *bounds;
} SceneCell;
/* A model the sweep may hit, and from how far along. */
typedef struct ModelHit {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ f32 t;
} ModelHit;
void Collision_LookAtF(float mf[4][4], float xEye, float yEye, float zEye, float xAt, float yAt, float zAt, float xUp, float yUp, float zUp);
f32 func_00298470(f32);                 /* sqrtf */
s32 Sweep_HitRect(Sweep *sweep, CRect *rect, f32 *t);
s32 Sweep_HitBox(Sweep *sweep, CBox *box, f32 *t);
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), void (*swap)());
s32 Actor_GetTypeClass(SceneModel *model);
void ModelSweep_TestModel(ModelSweep *ms);
/* qsort comparator over records with a float key at 4. */
/* the records sorted are {pointer, f32} (ModelHit, InstanceHit): the key after a host pointer */
typedef struct {
    void *unk0;
    f32 key;
} KeyedRecord;
s32 Collision_SortCompare(KeyedRecord *arg0, KeyedRecord *arg1);
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023F43C_de_7.c, func_802405FC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
void Collision_SortSwap(HitSortRecord *left, HitSortRecord *right);

/* The collision test against the scene's models: gathers the models whose
 * bounds the sweep crosses, nearest first, and sweeps against each until a
 * hit nearer than the next one. */
void Collision_TestModels(Sweep *sweep) {
    CBox box;
    CRect rect;
    ModelSweep ms;
    ModelHit hits[128];
    f32 t;
    CVector3 *end;
    CVector3 *start;
    s32 cellCount;
    SceneCell *cells;
    SceneCell *cell;
    SceneModels *list;
    SceneModel *model;
    ModelHit *hit;
    s32 i;
    s32 n;
    s32 j;

    if (!(sweep->def->m_dwFlags & 0x01000000)) {
        return;
    }
    if (sweep->def->m_WallBehavior == 0) {
        return;
    }
    end = &sweep->end;
    start = &sweep->inst->m_vPos;
    if (start->x == end->x && start->y == end->y && start->z == end->z) {
        return;
    }
    ms.sweep = sweep;
    Collision_LookAtF(ms.view, start->x, start->y, start->z, end->x, end->y, end->z, 0.0f, 1.0f, 0.0f);
    ms.scale = -func_00298470(sweep->delta.x * sweep->delta.x + sweep->delta.y * sweep->delta.y
                              + sweep->delta.z * sweep->delta.z);
    ms.hitDist = ms.scale * sweep->hit.fraction;
    box.m_vMin.x = MIN(end->x, start->x);
    box.m_vMax.x = MAX(end->x, start->x);
    box.m_vMin.y = MIN(end->y, start->y);
    box.m_vMax.y = MAX(end->y, start->y);
    box.m_vMin.z = MIN(end->z, start->z);
    box.m_vMax.z = MAX(end->z, start->z);
    rect.minX = box.m_vMin.x;
    rect.maxX = box.m_vMax.x;
    rect.maxZ = box.m_vMax.z;
    rect.minZ = box.m_vMin.z;
    hit = hits;
    i = 0;
    n = 0;
    cells = (SceneCell *)gScene.m_Sectors;
    cellCount = gScene.m_nSectors;
    for (; i < cellCount && !sweep->stop; i++) {
        cell = &cells[i];
        if (cell->bounds->maxX > rect.minX && cell->bounds->minX < rect.maxX && cell->bounds->maxZ > rect.minZ
            && cell->bounds->minZ < rect.maxZ && Sweep_HitRect(sweep, cell->bounds, &t)) {
            list = CISet__GetBlock(*cell->data, 0);
            j = list->count;
            model = list->models;
            while (--j != -1 && !sweep->stop && n < 128) {
                if (!(model->bounds.flags & 0x10) && box.m_vMax.x > model->bounds.box.m_vMin.x
                    && box.m_vMin.x < model->bounds.box.m_vMax.x && box.m_vMax.z > model->bounds.box.m_vMin.z
                    && box.m_vMin.z < model->bounds.box.m_vMax.z && box.m_vMax.y > model->bounds.box.m_vMin.y
                    && box.m_vMin.y < model->bounds.box.m_vMax.y && Sweep_HitBox(sweep, &model->bounds.box, &t)) {
                    n++;
                    hit->model = model;
                    hit->t = t;
                    hit++;
                }
                model++;
            }
        }
    }
    if (n != 0) {
        Sort_Quick(hits, n, sizeof(ModelHit), Collision_SortCompare, Collision_SortSwap);
        hit = hits;
        for (j = 0; j < n; j++, hit++) {
            if (!(hit->t < sweep->hit.fraction)) {
                return;
            }
            model = hit->model;
            ms.model = model;
            ms.kind = Actor_GetTypeClass(model);
            if (ms.kind == 9) {
                ms.special = 1;
            } else {
                ms.special = 0;
            }
            ModelSweep_TestModel(&ms);
        }
    }
}

#include "globals/collision.h"

/* A triangle corner projected to the sweep's space, with its model vertex. */
typedef struct ProjVertex {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ s32 flags;
    /* 0x10 */ s16 *pos;
} ProjVertex;
/* The scene model a ModelSweep is against: its data at 0x10. */
typedef struct SweptModel {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ void *data;
} SweptModel;
#include "protos/mtxf.h"

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *point, s32 arg3, s32 arg4, f32 arg5, s32 arg6);

/* Sweep against one triangle of a model, given by its corners projected so
 * that the sweep's line is the origin: if the line passes through the front
 * of the triangle, the triangle is put in world space and the hit recorded. */
void ModelSweep_TestTriangle(ModelSweep *ms, ProjVertex **v) {
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
        CMtxF__TransformPoint(ms->mtx, &pos, &world[j]);
    }
    CVector3__Subtract(&edge1, &world[1], &world[0]);
    CVector3__Subtract(&edge2, &world[2], &world[1]);
    CVector3__Cross(&normal, &edge2, &edge1);
    sweep = ms->sweep;
    def = sweep->def;
    switch (ms->kind) {
        case 1:
        case 7:
        case 8:
            if ((def->m_dwFlags & 8) && gCollisionResultsP->m_pWaterRegion == 0 &&
                Sweep_HitPlane(sweep, &normal, &world[0], 1, 7, def->m_WaterFriction, 0)) {
                sweep->hit.posOut = &gCollisionResultsP->m_vWaterPos;
                sweep->hit.regionOut = NULL;
                sweep->hit.kindOut = &gCollisionResultsP->m_nWaterKind;
                sweep->hit.kind = ms->kind;
                sweep->hit.dataOut = (void **)&gCollisionResultsP->m_pWaterRegion;
                sweep->hit.data = ((SweptModel *)ms->model)->data;
            }
            break;
        default:
            if (Sweep_HitPlane(sweep, &normal, &world[0], 1, def->m_WallBehavior, def->m_GroundFriction, 0)) {
                ms->hitDist = ms->scale * sweep->hit.fraction;
                sweep->hit.posOut = &gCollisionResultsP->m_vInstancePos;
                sweep->hit.kindOut = &gCollisionResultsP->m_nInstanceNode;
                sweep->hit.regionOut = NULL;
                sweep->hit.kind = -1;
                sweep->hit.dataOut = (void **)&gCollisionResultsP->m_pInstance;
                sweep->hit.data = ms->model;
                if (def->m_dwFlags & 0x20000000) {
                    sweep->stop = 1;
                }
            }
            break;
    }
}

#include "gfx.h"
#include "gbi_t2.h"
#include "structs/vector.h"
/* One command of a model part's display list (Model_Decompress's output). The port's Gfx
 * has host-sized words in host byte order, and Model_Decompress marks segment addresses
 * with bit 0 (include/gbi_t2.h): read the opcode and the segment offset with shifts. */
#ifdef TARGET_PORT
typedef Gfx DLCmd;
#define DLCMD_OP(c) ((u8) ((c)->w0 >> 24))
#define DLCMD_SEGOFF(c) ((c)->w1 & 0xFFFFFE)
#else
typedef union DLCmd {
    struct { u32 w0; u32 w1; };
    u8 b[8];
} DLCmd;
#define DLCMD_OP(c) ((c)->b[0])
#define DLCMD_SEGOFF(c) ((c)->w1 & 0xFFFFFF)
#endif

/* Walk one polygon group's display list (F3DEX2 G_VTX/G_TRI1/G_TRI2) and
 * sweep against every triangle that is not wholly outside the sweep. */
void ModelSweep_TestGroup(ModelSweep *ms, s32 *group) {
    ProjVertex verts[32];
    ProjVertex *tri[3];
    CVector3 pos;
    s32 doubleSided;
    u8 op;
    Sweep *sweep;
    u32 *header;
    DLCmd *cmd;
    ProjVertex *v;
    ProjVertex *swap;
    s16 *src;
    s32 n;
    s32 flags;
    f32 z;

    header = (u32 *)CISet__GetBlock(group, 0);
    if (ms->special == 0 && (*header & 0x280)) {
        return;
    }
    doubleSided = *header & 4;
    sweep = ms->sweep;
    cmd = (DLCmd *)CISet__GetBlock(group, 1);
    do {
        if (sweep->stop != 0) {
            return;
        }
        op = DLCMD_OP(cmd);
        switch (op) {
            case G_TRI1:
            case G_TRI2:
                tri[0] = &verts[(cmd->w0 & 0xFE0000) >> 17];
                tri[1] = &verts[(cmd->w0 & 0xFE00) >> 9];
                tri[2] = &verts[(cmd->w0 & 0xFF) >> 1];
                if (!(tri[0]->flags & tri[1]->flags & tri[2]->flags)) {
                    ModelSweep_TestTriangle(ms, tri);
                    if (doubleSided) {
                        swap = tri[0];
                        tri[0] = tri[2];
                        tri[2] = swap;
                        ModelSweep_TestTriangle(ms, tri);
                    }
                }
                if (op == G_TRI2) {
                    tri[0] = &verts[(cmd->w1 & 0xFE0000) >> 17];
                    tri[1] = &verts[(cmd->w1 & 0xFE00) >> 9];
                    tri[2] = &verts[(cmd->w1 & 0xFF) >> 1];
                    if (!(tri[0]->flags & tri[1]->flags & tri[2]->flags)) {
                        ModelSweep_TestTriangle(ms, tri);
                        if (doubleSided) {
                            swap = tri[0];
                            tri[0] = tri[2];
                            tri[2] = swap;
                            ModelSweep_TestTriangle(ms, tri);
                        }
                    }
                }
                break;
            case G_VTX:
                src = (s16 *)(DLCMD_SEGOFF(cmd) + GBI_ADDR(ms->meshes));
                n = (cmd->w0 >> 12) & 0xFF;
                v = &verts[((cmd->w0 & 0xFE) >> 1) - n];
                do {
                    v->pos = src;
                    pos.x = src[0];
                    pos.y = src[1];
                    pos.z = src[2];
                    CMtxF__TransformPoint(ms->inverse, &pos, (CVector3 *) v);
                    flags = 2;
                    if (v->x < 0.0f) {
                        flags = 1;
                    }
                    if (v->y < 0.0f) {
                        flags |= 8;
                    } else {
                        flags |= 4;
                    }
                    z = v->z;
                    v->flags = flags;
                    if (z >= 0.0f) {
                        v->flags = flags | 0x10;
                    } else if (z < ms->hitDist) {
                        v->flags = flags | 0x20;
                    }
                    src += 8;
                    v++;
                } while (--n != 0);
                break;
        }
        cmd++;
    } while (op != G_ENDDL);
}

void Collision_SortSwap(HitSortRecord *left, HitSortRecord *right) {
    HitSortRecord temporary = *left;
    *left = *right;
    *right = temporary;
}

s32 Collision_SortCompare(KeyedRecord *arg0, KeyedRecord *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (arg0->key < arg1->key) {
        var_v0 = -1;
    }
    return var_v0;
}

#include "protos/cache.h"
#include "structs/level_actor.h"
#include "structs/intelligence.h"

void CMtxF__MultiplyAffine(f32 (*dst)[4], f32 (*a)[4], f32 (*b)[4]);
void CMtxF__FromMtxAffine(f32 (*mtx)[4], void *packed);
s32 *CStaticObject__LoadGeometry(void *model);

/* Sweep against every polygon group of the model's current variant, until a hit. */
void ModelSweep_TestModel(ModelSweep *ms) {
    CStaticObject *model;
    s32 *data;
    s32 *root;
    s32 *groups;
    s32 count;
    Sweep *sweep;
    s32 i;

    model = ms->model;
    data = CStaticObject__LoadGeometry(model);
    if (data != NULL) {
        CMtxF__FromMtxAffine(ms->mtx, model->m_Matrix);
        CMtxF__MultiplyAffine(ms->inverse, ms->mtx, ms->view);
        root = *(s32 **)data; /* the cache entry holds a host pointer */
        ms->meshes = CISet__GetBlock(root, 0);
        groups = (s32 *)CISet__GetBlock((s32 *)CISet__GetBlock(root, 2), model->header.m_pIntelligence->m_nModel);
        count = *groups;
        sweep = ms->sweep;
        for (i = 0; i < count && sweep->stop == 0; i++) {
            ModelSweep_TestGroup(ms, CISet__GetBlock(groups, i));
        }
        CCache__Unlock(0, data);
    }
}

extern f32 func_00298470(f32);
extern void guMtxIdentF(float mf[4][4]);
extern float func_00298470(float);

void Collision_LookAtF(float mf[4][4], float xEye, float yEye, float zEye,
                   float xAt, float yAt, float zAt,
                   float xUp, float yUp, float zUp)
{
    float one, len, xLook, yLook, zLook, xRight, yRight, zRight;
    guMtxIdentF(mf);
    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    len = -1.0f / func_00298470(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;
    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    if (xRight == 0.0f && yRight == 0.0f && zRight == 0.0f) {
        xRight = 1.0f;
    }
    one = 1.0f;
    len = one / func_00298470(xRight * xRight + yRight * yRight + zRight * zRight);
    xRight *= len;
    yRight *= len;
    zRight *= len;
    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    len = one / func_00298470(xUp * xUp + yUp * yUp + zUp * zUp);
    xUp *= len;
    yUp *= len;
    zUp *= len;
    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);
    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);
    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);
    mf[0][3] = 0;
    mf[1][3] = 0;
    mf[2][3] = 0;
    mf[3][3] = one;
}

f32 GameObject_GetCollisionHeight(CGameObjectInstance *inst);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *inst);
f32 GameObject_GetCollisionYOffset(CGameObjectInstance *inst);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 Sweep_HitCylinder(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside);
s32 Sweep_IsSpecialObject(CObjectHead *other, CObjectHead *inst);
void SweepHit_Clear(f32 *hit);
void Platform_LandOnTop(Sweep *sweep, CObjectHead *other);
void Platform_Collide(Sweep *sweep, CObjectHead *other);

/* Sweep an instance's volume (box) against another instance's cylinder:
 * its top and bottom discs when the box asks for it, then its side. */
void Sweep_TestInstance(Sweep *sweep, SweepBox *box, CObjectHead *other) {
    CRect rect;
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
    CObjectHead *inst;
    CCollisionInfo *def;
    CCollisionResults *target;
    s32 result;
    s32 special;

    x = other->header.m_vPos.x;
    z = other->header.m_vPos.z;
    radius = GameObject_GetCollisionRadius((CGameObjectInstance *) other);
    rect.minX = x - radius;
    rect.maxX = x + radius;
    rect.minZ = z - radius;
    rect.maxZ = z + radius;
    if (!(rect.minX < box->maxX) || !(box->minX < rect.maxX) || !(rect.minZ < box->maxZ) ||
        !(box->minZ < rect.maxZ)) {
        return;
    }
    inst = (CObjectHead *)sweep->inst;
    def = sweep->def;
    special = 0;
    if (inst->header.m_Type == 1 && inst->header.m_pIntelligence->m_Type == 2) {
        if (other->header.m_Type == inst->header.m_Type) {
            Platform_LandOnTop(sweep, other);
        }
        return;
    }
    if (other->header.m_Type == 1 && other->header.m_pIntelligence->m_Type == 2) {
        if (inst->header.m_Type == other->header.m_Type) {
            Platform_Collide(sweep, other);
        }
        return;
    }
    if (other->header.m_Type == 3) {
        if (!Sweep_IsSpecialObject(other, inst)) {
            return;
        }
        special = 1;
    }
    bottom = other->header.m_vPos.y + GameObject_GetCollisionYOffset((CGameObjectInstance *) other);
    top = GameObject_GetCollisionHeight((CGameObjectInstance *) other) + bottom;
    if (box->flag) {
        normal.x = normal.z = 0.0f;
    }
    result = 0;
    reach = box->radius + radius;
    if (box->flag && sweep->delta.y != 0.0f) {
        if (sweep->lastKind == 2 && sweep->lastRadius == reach && sweep->lastContact.x == other->header.m_vPos.x &&
            sweep->lastContact.y == other->header.m_vPos.y && sweep->lastContact.z == other->header.m_vPos.z) {
            return;
        }
        if (box->startTop <= bottom && bottom < box->endTop) {
            t = (bottom - box->startTop) / sweep->delta.y;
            if (t < sweep->hit.fraction) {
                CVector3__Lerp(&hit, t, &inst->header.m_vPos, &sweep->end);
                dx = x - hit.x;
                dz = z - hit.z;
                if (dx * dx + dz * dz < reach * reach) {
                    normal.y = -1.0f;
                    result |= Sweep_HitPlane(sweep, &normal, &hit, 0, 7, 0.0f, 1);
                }
            }
        }
        if (top <= box->startBottom && box->endBottom < top) {
            t = (top - box->startBottom) / sweep->delta.y;
            if (t < sweep->hit.fraction) {
                CVector3__Lerp(&hit, t, &inst->header.m_vPos, &sweep->end);
                dx = x - hit.x;
                dz = z - hit.z;
                if (dx * dx + dz * dz < reach * reach) {
                    normal.y = 1.0f;
                    if (other->header.m_pIntelligence->m_dwFlags & 0x80) {
                        if (Sweep_HitPlane(sweep, &normal, &hit, 0, 2, 0.0f, 1)) {
                            result |= 1;
                            if (inst->m_vVelocity.y < 0.0f) {
                                inst->m_vVelocity.y = 0.0f;
                                inst->m_pInstanceBelow = (CGameObjectInstance *)other;
                            }
                        }
                    } else {
                        result |= Sweep_HitPlane(sweep, &normal, &hit, 0, 7, 0.0f, 1);
                    }
                }
            }
        }
    }
    result |= Sweep_HitCylinder(sweep, &other->header.m_vPos, reach, 1, top - box->bottom, bottom - box->top, 1,
                            special ? 7 : def->m_InstanceBehavior, 0.0f, 1);
    if (result) {
        target = gCollisionResultsP;
        if (target->m_pInstance) {
            SweepHit_Clear(&sweep->hit.fraction);
        } else {
            sweep->hit.posOut = &target->m_vInstancePos;
            sweep->hit.regionOut = &target->m_pInstanceRegion;
            sweep->hit.kindOut = &target->m_nInstanceNode;
            sweep->hit.kind = -1;
            sweep->hit.dataOut = (void **)&target->m_pInstance;
            sweep->hit.data = other;
        }
        if (box->flag) {
            sweep->kind = 2;
            sweep->contact = other->header.m_vPos;
            sweep->radius = reach;
        }
    }
}

#include "structs/world.h"
#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* Sweep an instance's volume against every drawn instance it may hit. */
void Collision_TestInstances(Sweep *sweep) {
    SweepBox box;
    SweepBox *b;
    CGameObjectInstance *inst;
    CCollisionInfo *def;
    CGameObjectInstance *other;
    CScene *world;
    s32 count;
    f32 x;
    f32 z;
    s32 i;

    def = sweep->def;
    if (def->m_InstanceBehavior == 0 || (def->m_dwFlags & 0x40000000) || def == &gCollisionInfoNoInstances) {
        return;
    }
    inst = sweep->obj;
    count = gScene_m_nDrawActors; /* gScene.drawCount */
    b = &box;
    box.radius = GameObject_GetCollisionRadius(inst);
    b->flag = def->m_dwFlags & 0x100000;
    b->bottom = GameObject_GetCollisionYOffset(inst);
    b->top = b->bottom + GameObject_GetCollisionHeight(inst);
    b->startBottom = inst->header.m_vPos.y + b->bottom;
    b->startTop = inst->header.m_vPos.y + b->top;
    b->endBottom = sweep->end.y + b->bottom;
    b->endTop = sweep->end.y + b->top;
    x = inst->header.m_vPos.x;
    z = inst->header.m_vPos.z;
    b->minX = MIN(sweep->end.x, x) - box.radius;
    b->maxX = MAX(sweep->end.x, x) + box.radius;
    b->minZ = MIN(sweep->end.z, z) - box.radius;
    b->maxZ = MAX(sweep->end.z, z) + box.radius;
    world = &gScene;
    for (i = 0; i < count; i++) {
        other = (CGameObjectInstance *)world->m_pDrawActors[i];
        if (other == inst || def->m_InstanceBehavior == 0) {
            continue;
        }
        switch (other->header.m_Type) {
            case 1:
                if ((def->m_dwFlags & 1) && other->m_AI.m_Health == 0) {
                    continue;
                }
                break;
            case 2:
                break;
            case 3:
                if (!(def->m_dwFlags & 0x20)) {
                    continue;
                }
                break;
        }
        Sweep_TestInstance(sweep, &box, other);
    }
}

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* Fill the bounds of the volume a sweep's instance moves through. */
void Sweep_GetVolumeBounds(Sweep *sweep, SweepBox *box) {
    CGameObjectInstance *inst;
    CCollisionInfo *def;
    f32 x;
    f32 z;

    inst = sweep->obj;
    def = sweep->def;
    box->radius = GameObject_GetCollisionRadius(inst);
    box->flag = def->m_dwFlags & 0x100000;
    box->bottom = GameObject_GetCollisionYOffset(inst);
    box->top = box->bottom + GameObject_GetCollisionHeight(inst);
    box->startBottom = inst->header.m_vPos.y + box->bottom;
    box->startTop = inst->header.m_vPos.y + box->top;
    box->endBottom = sweep->end.y + box->bottom;
    box->endTop = sweep->end.y + box->top;
    x = inst->header.m_vPos.x;
    z = inst->header.m_vPos.z;
    box->minX = MIN(sweep->end.x, x) - box->radius;
    box->maxX = MAX(sweep->end.x, x) + box->radius;
    box->minZ = MIN(sweep->end.z, z) - box->radius;
    box->maxZ = MAX(sweep->end.z, z) + box->radius;
}
