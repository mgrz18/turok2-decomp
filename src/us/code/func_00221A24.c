#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Bounds {
    Vec3 min;                   /* 0x00 */
    Vec3 max;                   /* 0x0C */
} Bounds;

typedef struct InstDef {
    s32 pad0;
    s32 mask;                   /* 0x04: section layers */
} InstDef;

typedef struct Instance {
    u8 pad0[0x14];
    InstDef *def;               /* 0x14 */
    u8 pad18[0x68 - 0x18];
    Bounds bounds;              /* 0x68 */
    u8 pad80[0x90 - 0x80];
} Instance;                     /* 0x90 bytes */

typedef struct LevelActor {
    u8 pad0[0x4];
    Vec3 pos;                   /* 0x004 */
    u8 pad10[0x14 - 0x10];
    InstDef *def;               /* 0x014 */
    u8 pad18[0x14C - 0x18];
    Bounds bounds;              /* 0x14C */
    u8 pad164[0x16C - 0x164];
    u16 flags;                  /* 0x16C */
    u8 pad16E[0x178 - 0x16E];
    u8 unk178[0x198 - 0x178];   /* 0x178 */
} LevelActor;                   /* 0x198 bytes */

typedef struct List {
    s32 pad0;
    s32 count;                  /* 0x04 */
    u8 items[1];                /* 0x08 */
} List;

typedef struct Region {
    void **data;                /* 0x00 */
    void *cell;                 /* 0x04 */
} Region;

typedef struct World {
    u8 pad0[0xEC];
    void *draw[0x200];          /* 0x0EC */
    s32 drawCount;              /* 0x8EC */
    u8 pad8F0[0x14A8 - 0x8F0];
    s32 regionCount;            /* 0x14A8 */
    Region regions[128];        /* 0x14AC */
    u8 pad18AC[0x10518 - 0x18AC];
    u8 unk10518[1];             /* 0x10518 */
} World;

extern s32 D_8011AAE0;          /* visible layers */
extern s32 D_800ACADC;
extern Bounds D_800F5A10;       /* visible bounds */

void *func_002017D4(void *table, s32 index);
void func_00214E88(Instance *inst);
s32 func_00267090(Instance *inst);
f32 func_002671B4(LevelActor *actor);
void func_002254C0(World *world, Vec3 *pos, void *out);
void func_002156FC(LevelActor *actor);
void func_002279A4(void *arg0, World *world);

/* Collects the instances and actors of the loaded sections inside the visible bounds. */
void func_00221A24(World *world) {
    Vec3 pos;
    s32 regionCount;
    void *data;
    Region *regions;
    List *list;
    Instance *inst;
    Instance *insts;
    LevelActor *actors;
    LevelActor *actor;
    s32 layers;
    s32 i;
    s32 j;
    s32 instCount;
    s32 actorCount;

    i = 0;
    regionCount = world->regionCount;
    layers = D_8011AAE0;
    regions = world->regions;
    for (; i < regionCount; i++) {
        data = *regions[i].data;
        j = 0;
        list = func_002017D4(data, 0);
        instCount = list->count;
        insts = (Instance *)list->items;
        for (; j < instCount; j++) {
            inst = &insts[j];
            if (!(inst->def->mask & layers)) {
                continue;
            }
            if (D_800F5A10.max.x > inst->bounds.min.x && D_800F5A10.min.x < inst->bounds.max.x
                && D_800F5A10.max.z > inst->bounds.min.z && D_800F5A10.min.z < inst->bounds.max.z
                && D_800F5A10.max.y > inst->bounds.min.y && D_800F5A10.min.y < inst->bounds.max.y) {
                func_00214E88(inst);
                if (func_00267090(inst) && world->drawCount != 0x200) {
                    world->draw[world->drawCount++] = inst;
                }
            }
        }
        list = func_002017D4(data, 1);
        j = 0;
        actorCount = list->count;
        actors = (LevelActor *)list->items;
        for (; j < actorCount; j++) {
            actor = &actors[j];
            if (!(actor->def->mask & layers)) {
                continue;
            }
            if (actor->flags & 0x41) {
                continue;
            }
            if (D_800ACADC != 0) {
                pos.x = actor->pos.x;
                pos.y = actor->pos.y + func_002671B4(actor) * 0.5f;
                pos.z = actor->pos.z;
                func_002254C0(world, &pos, actor->unk178);
            }
            if (D_800F5A10.max.x > actor->bounds.min.x && D_800F5A10.min.x < actor->bounds.max.x
                && D_800F5A10.max.z > actor->bounds.min.z && D_800F5A10.min.z < actor->bounds.max.z
                && D_800F5A10.max.y > actor->bounds.min.y && D_800F5A10.min.y < actor->bounds.max.y) {
                func_002156FC(actor);
                if (world->drawCount != 0x200) {
                    world->draw[world->drawCount++] = actor;
                }
            }
        }
    }
    func_002279A4(world->unk10518, world);
}
