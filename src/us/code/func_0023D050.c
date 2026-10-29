#include "common.h"
#include "structs/game_object.h"

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* A box: min corner then max corner. */
typedef struct Box {
    /* 0x00 */ CVector3 min;
    /* 0x0C */ CVector3 max;
} Box;

/* A rectangle on the ground plane. */
typedef struct Rect {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} Rect;

typedef struct CollisionDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ s8 unk5;
} CollisionDef;

/* A segment swept from an instance's position to end (see func_00239F00). */
typedef struct Sweep {
    /* 0x00 */ CInstanceHeader *obj;
    /* 0x04 */ CollisionDef *def;
    /* 0x08 */ f32 fraction;
    /* 0x0C */ u8 padC[0x40];
    /* 0x4C */ CVector3 end;
    /* 0x58 */ CVector3 delta;
    /* 0x64 */ u8 pad64[0x38];
    /* 0x9C */ s32 stop;
} Sweep;

/* A sweep against a model, in the sweep's own space (see func_0023DBBC). */
typedef struct ModelSweep {
    /* 0x00 */ Sweep *sweep;
    /* 0x04 */ f32 view[4][4];          /* world to sweep space */
    /* 0x44 */ f32 scale;
    /* 0x48 */ f32 hitDist;
    /* 0x4C */ void *model;
    /* 0x50 */ s32 kind;
    /* 0x54 */ f32 mtx[4][4];
    /* 0x94 */ f32 inverse[4][4];
    /* 0xD4 */ void *meshes;
    /* 0xD8 */ s32 special;
} ModelSweep;

/* A scene model's bounds (what func_0023A640 tests). */
typedef struct ModelBounds {
    /* 0x00 */ Box box;
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
s32 func_0023A640(Sweep *sweep, Box *box, f32 *t);
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
    Box box;
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

    if (!(sweep->def->flags & 0x01000000)) {
        return;
    }
    if (sweep->def->unk5 == 0) {
        return;
    }
    end = &sweep->end;
    start = &sweep->obj->m_vPos;
    if (start->x == end->x && start->y == end->y && start->z == end->z) {
        return;
    }
    ms.sweep = sweep;
    func_0023DCBC(ms.view, start->x, start->y, start->z, end->x, end->y, end->z, 0.0f, 1.0f, 0.0f);
    ms.scale = -func_00298470(sweep->delta.x * sweep->delta.x + sweep->delta.y * sweep->delta.y
                              + sweep->delta.z * sweep->delta.z);
    ms.hitDist = ms.scale * sweep->fraction;
    box.min.x = MIN(end->x, start->x);
    box.max.x = MAX(end->x, start->x);
    box.min.y = MIN(end->y, start->y);
    box.max.y = MAX(end->y, start->y);
    box.min.z = MIN(end->z, start->z);
    box.max.z = MAX(end->z, start->z);
    rect.minX = box.min.x;
    rect.maxX = box.max.x;
    rect.maxZ = box.max.z;
    rect.minZ = box.min.z;
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
                if (!(model->bounds.flags & 0x10) && box.max.x > model->bounds.box.min.x
                    && box.min.x < model->bounds.box.max.x && box.max.z > model->bounds.box.min.z
                    && box.min.z < model->bounds.box.max.z && box.max.y > model->bounds.box.min.y
                    && box.min.y < model->bounds.box.max.y && func_0023A640(sweep, &model->bounds.box, &t)) {
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
            if (!(hit->t < sweep->fraction)) {
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
