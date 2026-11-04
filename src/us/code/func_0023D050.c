#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* A rectangle on the ground plane. */
typedef struct Rect {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} Rect;

/* A scene model's bounds (what func_0023A640 tests). */
typedef struct ModelBounds {
    /* 0x00 */ CBox box;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ u16 flags;               /* 0x10: no collisions */
    /* 0x22 */ u8 pad22[6];
} ModelBounds;

/* One model instance of the scene, as listed for collisions. */
typedef struct SceneModel {
    /* 0x00 */ u8 data[0x68];
    /* 0x68 */ ModelBounds bounds;
} SceneModel; /* size 0x90 */

typedef struct SceneModels {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 count;
    /* 0x08 */ SceneModel models[1];
} SceneModels;

/* A cell of the scene grid: its models and their bounds. */
typedef struct SceneCell {
    /* 0x00 */ s32 **data;
    /* 0x04 */ Rect *bounds;
} SceneCell;

typedef struct SceneCells {
    /* 0x00 */ s32 count;
    /* 0x04 */ SceneCell cells[1];
} SceneCells;

/* A model the sweep may hit, and from how far along. */
typedef struct ModelHit {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ f32 t;
} ModelHit;

extern SceneCells D_800F8520;

void func_0023DCBC(f32 mf[4][4], f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
                   f32 zUp);
f32 func_00298470(f32);                 /* sqrtf */
s32 func_0023A968(Sweep *sweep, Rect *rect, f32 *t);
s32 func_0023A640(Sweep *sweep, CBox *box, f32 *t);
SceneModels *func_002017D4(s32 *list, s32 index);
void func_00226F90(void *base, s32 count, s32 size, s32 (*cmp)(), void (*swap)());
s32 func_0023DB98();
void func_0023DB5C();
s32 func_00245724(SceneModel *model);
void func_0023DBBC(ModelSweep *ms);

/* The collision test against the scene's models: gathers the models whose
 * bounds the sweep crosses, nearest first, and sweeps against each until a
 * hit nearer than the next one. */
void func_0023D050(Sweep *sweep) {
    CBox box;
    Rect rect;
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
    func_0023DCBC(ms.view, start->x, start->y, start->z, end->x, end->y, end->z, 0.0f, 1.0f, 0.0f);
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
    cells = D_800F8520.cells;
    cellCount = D_800F8520.count;
    for (; i < cellCount && !sweep->stop; i++) {
        cell = &cells[i];
        if (cell->bounds->maxX > rect.minX && cell->bounds->minX < rect.maxX && cell->bounds->maxZ > rect.minZ
            && cell->bounds->minZ < rect.maxZ && func_0023A968(sweep, cell->bounds, &t)) {
            list = func_002017D4(*cell->data, 0);
            j = list->count;
            model = list->models;
            while (--j != -1 && !sweep->stop && n < 128) {
                if (!(model->bounds.flags & 0x10) && box.m_vMax.x > model->bounds.box.m_vMin.x
                    && box.m_vMin.x < model->bounds.box.m_vMax.x && box.m_vMax.z > model->bounds.box.m_vMin.z
                    && box.m_vMin.z < model->bounds.box.m_vMax.z && box.m_vMax.y > model->bounds.box.m_vMin.y
                    && box.m_vMin.y < model->bounds.box.m_vMax.y && func_0023A640(sweep, &model->bounds.box, &t)) {
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
        func_00226F90(hits, n, sizeof(ModelHit), func_0023DB98, func_0023DB5C);
        hit = hits;
        for (j = 0; j < n; j++, hit++) {
            if (!(hit->t < sweep->hit.fraction)) {
                return;
            }
            model = hit->model;
            ms.model = model;
            ms.kind = func_00245724(model);
            if (ms.kind == 9) {
                ms.special = 1;
            } else {
                ms.special = 0;
            }
            func_0023DBBC(&ms);
        }
    }
}
