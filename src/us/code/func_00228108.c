#include "common.h"
#include "structs/game_object.h"

/* Spawns an actor from loose parameters: takes a free instance off the
 * world's free list (counted in *owner) and initialises it. */

extern void func_00219BC0(CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence,
                          s32 sound, void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale,
                          CVector3 velocity, s32 extra);
extern s32 func_00224180(void *level, s32 type);
extern void func_0021E5D4(CGameObjectInstance *obj);
extern void CList__AddHead(void *list, void *node);
extern void CList__Remove(void *list, void *node);
extern s32 D_800B6D00;
extern s32 D_800B6D54;
extern u8 D_800F7078[];
extern s32 D_8011001C;

/* The world's instance pool: free and active lists of actors. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x5300];
    /* 0x5300 */ CGameObjectInstance *freeHead; /* CList of free instances */
    /* 0x5304 */ u8 pad5304[0x5314 - 0x5304];
    /* 0x5314 */ u8 activeList[0x5324 - 0x5314];
    /* 0x5324 */ u32 freeCount;
} CInstancePool;

/* A pooled instance: the game object plus the counter it was spawned for. */
typedef struct {
    /* 0x000 */ CGameObjectInstance obj;
    /* 0x288 */ u8 pad288[0x290 - 0x288];
    /* 0x290 */ s32 *owner;
} CPooledInstance;

/* func_002283FC */
static __inline__ CGameObjectInstance *take_free_instance(CInstancePool *world, s32 *owner) {
    CGameObjectInstance *node;

    if (D_8011001C != 0 || world->freeCount < 3) {
        node = world->freeHead;
        if (node != NULL) {
            CList__Remove(&world->freeHead, node);
            CList__AddHead(world->activeList, node);
            ((CPooledInstance *)node)->owner = owner;
            if (owner != NULL) {
                *owner += 1;
            }
        }
        return node;
    }
    return NULL;
}

CGameObjectInstance *func_00228108(void *world, s32 model, s32 type, void *intelligence, void *region,
                                   CVector3 pos, f32 rotY, CVector3 velocity, CVector3 scale, s32 *owner) {
    CGameObjectInstance *obj;

    if (D_800B6D54 == 0) {
        return NULL;
    }
    if (intelligence == NULL) {
        return NULL;
    }
    if (model == -1) {
        model = func_00224180(D_800F7078, type);
        if (model == -1) {
            return NULL;
        }
    }
    obj = take_free_instance(world, owner);
    if (obj == NULL) {
        return NULL;
    }
    func_00219BC0(obj, model, type, D_800B6D00, intelligence, -1, region, rotY, pos, 0x80000, scale,
                  velocity, 0);
    func_0021E5D4(obj);
    return obj;
}
