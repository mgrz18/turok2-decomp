#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"
#include "globals/camera.h"

/* Spawns an actor from loose parameters: takes a free instance off the
 * world's free list (counted in *owner) and initialises it. */

extern void CGameObjectInstance__Construct(CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence,
                          s32 sound, void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale,
                          CVector3 velocity, s32 extra);
extern s32 CScene__FindObjectType(void *level, s32 type);
extern void func_0021E5D4(CGameObjectInstance *obj);
extern void CList__AddHead(void *list, void *node);
extern void CList__Remove(void *list, void *node);
extern u8 gScene[];
extern s32 gScene_m_bStreamAll;

/* CInstancePool__Alloc */
static __inline__ CGameObjectInstance *take_free_instance(CInstancePool *world, s32 *owner) {
    CGameObjectInstance *node;

    if (gScene_m_bStreamAll != 0 || (u32)world->m_ActiveList.m_nCount < 3) {
        node = world->m_FreeList.m_pHead;
        if (node != NULL) {
            CList__Remove(&world->m_FreeList, node);
            CList__AddHead(&world->m_ActiveList, node);
            ((CListedInstance *)node)->m_pOwnerCount = owner;
            if (owner != NULL) {
                *owner += 1;
            }
        }
        return node;
    }
    return NULL;
}

CGameObjectInstance *CInstancePool__Spawn(void *world, s32 model, s32 type, void *intelligence, void *region,
                                   CVector3 pos, f32 rotY, CVector3 velocity, CVector3 scale, s32 *owner) {
    CGameObjectInstance *obj;

    if (gPersistentDataLoaded == 0) {
        return NULL;
    }
    if (intelligence == NULL) {
        return NULL;
    }
    if (model == -1) {
        model = CScene__FindObjectType(gScene, type);
        if (model == -1) {
            return NULL;
        }
    }
    obj = take_free_instance(world, owner);
    if (obj == NULL) {
        return NULL;
    }
    CGameObjectInstance__Construct(obj, model, type, gDefaultVisBits, intelligence, -1, region, rotY, pos, 0x80000, scale,
                  velocity, 0);
    func_0021E5D4(obj);
    return obj;
}
