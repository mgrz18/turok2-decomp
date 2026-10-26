#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Bounds {
    Vec3 min;                   /* 0x00 */
    Vec3 max;                   /* 0x0C */
} Bounds;

/* A level actor spawned at run time (a pickup), kept in the world's pool. */
typedef struct PoolActor {
    /* 0x000 */ u8 pad0[0x2];
    /* 0x002 */ s16 index;              /* in the level's id table */
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ void *def;
    /* 0x018 */ Vec3 vel;
    /* 0x024 */ s32 unk24[4];
    /* 0x034 */ s32 resource;
    /* 0x038 */ u8 pad38[0x138 - 0x38];
    /* 0x138 */ s32 unk138;
    /* 0x13C */ f32 scale;
    /* 0x140 */ s32 unk140;
    /* 0x144 */ s32 unk144;
    /* 0x148 */ s32 unk148;
    /* 0x14C */ Bounds bounds;
    /* 0x164 */ s32 unk164;
    /* 0x168 */ u8 pad168[0x4];
    /* 0x16C */ u16 flags;
    /* 0x16E */ u8 pad16E[0x178 - 0x16E];
    /* 0x178 */ u8 unk178[0x194 - 0x178];
    /* 0x194 */ u8 frame194;
    /* 0x195 */ u8 frame195;
    /* 0x196 */ u8 pad196[0x2];
    /* 0x198 */ f32 lifetime;
    /* 0x19C */ s32 timer;
    /* 0x1A0 */ s32 poolFlags;          /* 1: in use */
    /* 0x1A4 */ s32 *refCount;
    /* 0x1A8 */ struct PoolActor *prev;
    /* 0x1AC */ struct PoolActor *next;
} PoolActor;                            /* 0x1B0 bytes */

typedef struct ActorPool {
    /* 0x0000 */ PoolActor items[32];
    /* 0x3600 */ PoolActor *free;
    /* 0x3604 */ PoolActor *head;       /* newest */
    /* 0x3608 */ PoolActor *tail;       /* oldest */
} ActorPool;

extern s32 D_800B6D54;
extern u8 D_800F7078[];                 /* the world */
extern u8 D_8011AAF1;
extern u8 D_800F12C0[];
extern u8 D_800F1360[];
extern u8 D_800B6D1B;                   /* frame counter */

s32 func_00224180(void *world, s32 id);
s32 func_00224F84(void *world, s32 index);
void func_00227B7C(ActorPool *pool, PoolActor *actor);
f32 func_002671B4(PoolActor *actor);
void func_002254C0(void *world, Vec3 *pos, void *out);

/* Takes a free slot, or releases the oldest actor for one, links it at the head and clears it. */
static __inline__ PoolActor *allocActor(ActorPool *pool) {
    PoolActor *actor;

    if (pool->free == NULL) {
        func_00227B7C(pool, pool->tail);
    }
    actor = pool->free;
    pool->free = actor->next;
    if (pool->head != NULL) {
        pool->head->prev = actor;
    }
    actor->next = pool->head;
    actor->prev = NULL;
    pool->head = actor;
    if (pool->tail == NULL) {
        pool->tail = actor;
    }
    actor->vel.x = 0;
    actor->vel.y = 0;
    actor->vel.z = 0;
    actor->unk24[0] = 0;
    actor->unk24[1] = 0;
    actor->unk24[2] = 0;
    actor->unk24[3] = 0;
    actor->lifetime = 0;
    actor->poolFlags |= 1;
    return actor;
}

/* Spawns a pool actor of the given level id at pos with velocity vel, taking a free slot (or
 * the oldest one, released first) and linking it at the head; counts it in *refCount. */
PoolActor *func_00227240(ActorPool *pool, s32 *refCount, s32 id, Vec3 vel, Vec3 pos, s32 arg9, f32 lifetime) {
    PoolActor *actor;
    Vec3 p;
    s32 index;
    s32 resource;

    if (D_800B6D54 == 0 || (index = func_00224180(D_800F7078, id)) == -1) {
        return NULL;
    }
    actor = allocActor(pool);
    resource = func_00224F84(D_800F7078, index);
    actor->refCount = refCount;
    if (refCount != NULL) {
        (*refCount)++;
    }
    if (D_8011AAF1) {
        actor->def = D_800F12C0;
    } else {
        actor->def = D_800F1360;
    }
    actor->lifetime = lifetime;
    actor->timer = 0.0f;
    actor->index = index;
    actor->pos = pos;
    actor->unk144 = 0;
    actor->unk148 = 0;
    actor->unk138 = 0;
    actor->unk140 = 0;
    actor->scale = 1.0f;
    actor->bounds.min.x = pos.x - 81.92f;
    actor->bounds.min.y = pos.y - 81.92f;
    actor->bounds.min.z = pos.z - 81.92f;
    actor->bounds.max.x = pos.x + 81.92f;
    actor->bounds.max.y = pos.y + 81.92f;
    actor->bounds.max.z = pos.z + 81.92f;
    actor->resource = resource;
    actor->unk10 = arg9;
    actor->flags = 0x1A;
    actor->unk164 = 0;
    actor->vel = vel;
    actor->frame194 = actor->frame195 = D_800B6D1B - 1;
    p.x = actor->pos.x;
    p.y = actor->pos.y + func_002671B4(actor) * 0.5f;
    p.z = actor->pos.z;
    func_002254C0(D_800F7078, &p, actor->unk178);
    return actor;
}
